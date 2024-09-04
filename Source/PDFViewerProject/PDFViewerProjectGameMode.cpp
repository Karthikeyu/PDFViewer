// Copyright Epic Games, Inc. All Rights Reserved.

#include "PDFViewerProjectGameMode.h"
#include "PDFViewerProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

APDFViewerProjectGameMode::APDFViewerProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
