// Copyright Epic Games, Inc. All Rights Reserved.

#include "PDF_Viewer.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FPDF_ViewerModule"

void FPDF_ViewerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#ifdef _WIN64
	const FString BasePluginDir = IPluginManager::Get().FindPlugin("PDF_VIEWER")->GetBaseDir();
	const FString DLL_Path = FPaths::Combine(*BasePluginDir, TEXT("Source/PDF_Viewer/pdfium/Windows/bin/pdfium.dll"));
	PDFium_Handle = FPlatformProcess::GetDllHandle(*DLL_Path);

	if (PDFium_Handle != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("pdfium.dll loaded successfully!"));
	}

	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("pdfium.dll failed to load!"));
	}
#endif
	
}

void FPDF_ViewerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

#ifdef _WIN64
	FPlatformProcess::FreeDllHandle(PDFium_Handle);
	PDFium_Handle = nullptr;
#endif
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPDF_ViewerModule, PDF_Viewer)