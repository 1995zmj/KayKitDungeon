// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonGameMode.h"
#include "DungeonCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADungeonGameMode::ADungeonGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
