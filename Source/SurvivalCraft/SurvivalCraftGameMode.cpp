// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivalCraftGameMode.h"
#include "SurvivalCraftCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASurvivalCraftGameMode::ASurvivalCraftGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
