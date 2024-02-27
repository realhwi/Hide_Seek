// Copyright Epic Games, Inc. All Rights Reserved.

#include "Hide_SeekGameMode.h"
#include "Hide_SeekCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHide_SeekGameMode::AHide_SeekGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
