// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "PDF_ViewerBPLibrary.generated.h"

#ifdef _WIN64
#include <Windows/AllowWindowsPlatformTypes.h>
#include "fpdfview.h"
#include <Windows/HideWindowsPlatformTypes.h>
#endif

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UPDF_ViewerBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()



	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PDF Viewer - Library Init", Keywords = "pdf, pdfium, library, lib, open"), Category = "PDF_Reader|System")
	static bool PDF_LibInit(FString& Out_Code);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PDF Viewer - Library Close", Keywords = "pdf, pdfium, library, lib, close"), Category = "PDF_Reader|System")
	static bool PDF_LibClose(FString& Out_Code);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PDF Viewer - convert PDF To Texture", ToolTip = "", Keywords = "pdf, pdfium, convert, texture"), Category = "PDF_Reader|Read")
	static bool ConvertPDFToTexture(const FString& InputFilePath, const FString& OutputFilePath, TMap<UTexture2D*, FVector2D>& OutTextures);

};
