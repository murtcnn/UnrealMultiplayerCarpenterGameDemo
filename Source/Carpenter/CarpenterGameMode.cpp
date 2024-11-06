// Copyright Epic Games, Inc. All Rights Reserved.

#include "CarpenterGameMode.h"
#include "CarpenterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACarpenterGameMode::ACarpenterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
