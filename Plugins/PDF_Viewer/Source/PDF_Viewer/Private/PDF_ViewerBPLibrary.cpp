// Copyright Epic Games, Inc. All Rights Reserved.

#include "PDF_ViewerBPLibrary.h"
#include "PDF_Viewer.h"
#include "../Pdfium/Windows/include/fpdfview.h"
#include <Engine/Texture.h>


static inline bool Global_bIsLibInitialized = false;

// Global documents pool.
static inline TSet<FPDF_DOCUMENT> Global_Docs_Pool;

UPDF_ViewerBPLibrary::UPDF_ViewerBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		AddToRoot();
	}
}

bool UPDF_ViewerBPLibrary::PDF_LibInit(FString &Out_Code)
{
    if (Global_bIsLibInitialized == true)
	{
		Out_Code = "Library already initialized";
		return false;
	}
	
	FPDF_LIBRARY_CONFIG config;
	FMemory::Memset(&config, 0, sizeof(config));

	config.version = 2;
	config.m_pUserFontPaths = NULL;
	config.m_pIsolate = NULL;
	config.m_v8EmbedderSlot = 0;
	FPDF_InitLibraryWithConfig(&config);

	Global_bIsLibInitialized = true;

	//DefineCharCodes();

	Out_Code = "Library successfully initialized.";
	return true;
}

bool UPDF_ViewerBPLibrary::PDF_LibClose(FString &Out_Code)
{
    if (Global_bIsLibInitialized == false)
	{
		Out_Code = "Library already closed.";
		return false;
	}
	
	TArray<FPDF_DOCUMENT> Array_Docs = Global_Docs_Pool.Array();
	for (int32 Index_Docs = 0; Index_Docs < Array_Docs.Num(); Index_Docs++)
	{
		if (Array_Docs[Index_Docs])
		{
			FPDF_CloseDocument(Array_Docs[Index_Docs]);
		}
	}
	
	Array_Docs.Empty();
	Global_Docs_Pool.Empty();

	Global_bIsLibInitialized = false;
	FPDF_DestroyLibrary();

	Out_Code = "Library successfully closed.";
	return true;
}

bool UPDF_ViewerBPLibrary::ConvertPDFToTexture(const FString &InputFilePath, const FString &OutputFilePath, TMap<UTexture2D *, FVector2D> &OutTextures)
{
     std::string FilePathStd = TCHAR_TO_UTF8(*InputFilePath);

	float ScaleFactor = 2.0f;
    // Load the PDF document
    FPDF_DOCUMENT pdfDoc = FPDF_LoadDocument(FilePathStd.c_str(), nullptr);
    if (!pdfDoc)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load PDF document."));
        return false;
    }

    // Get the number of pages in the PDF
    int PageCount = FPDF_GetPageCount(pdfDoc);
    if (PageCount <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No pages found in the PDF document."));
        FPDF_CloseDocument(pdfDoc);
        return false;
    }

    // Loop through all the pages
    for (int PageIndex = 0; PageIndex < PageCount; ++PageIndex)
    {
        // Load the specified page
        FPDF_PAGE page = FPDF_LoadPage(pdfDoc, PageIndex);
        if (!page)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load PDF page %d."), PageIndex);
            continue;
        }

        // Get the page dimensions
        int OriginalWidth = static_cast<int>(FPDF_GetPageWidth(page));
        int OriginalHeight = static_cast<int>(FPDF_GetPageHeight(page));

        // Scale dimensions by the scale factor for high-resolution rendering
        int Width = static_cast<int>(OriginalWidth * ScaleFactor);
        int Height = static_cast<int>(OriginalHeight * ScaleFactor);

        // Create a bitmap to render the page at a higher resolution
        FPDF_BITMAP bitmap = FPDFBitmap_Create(Width, Height, 0);
        FPDFBitmap_FillRect(bitmap, 0, 0, Width, Height, 0xFFFFFFFF); // Fill with white color
        FPDF_RenderPageBitmap(bitmap, page, 0, 0, Width, Height, 0, 0);

        // Get the raw bitmap buffer
        void* Buffer = FPDFBitmap_GetBuffer(bitmap);

        // Convert the raw buffer to a UTexture2D
        UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);

        if (Texture)
        {
            uint8* MipData = static_cast<uint8*>(Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
            FMemory::Memcpy(MipData, Buffer, Width * Height * 4);
            Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

            // Update the texture resource to apply the changes
            Texture->UpdateResource();

            // Store the texture and its resolution in the map
            OutTextures.Add(Texture, FVector2D(OriginalWidth, OriginalHeight));  // Store the original resolution for UI or other uses
        }

        // Clean up PDFium resources for the current page
        FPDFBitmap_Destroy(bitmap);
        FPDF_ClosePage(page);
    }

    // Clean up PDFium resources for the document
    FPDF_CloseDocument(pdfDoc);

	return true;
}
