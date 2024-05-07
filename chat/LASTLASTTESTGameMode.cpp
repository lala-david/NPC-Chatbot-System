// Copyright Epic Games, Inc. All Rights Reserved.

#include "LASTLASTTESTGameMode.h"
#include "LASTLASTTESTCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALASTLASTTESTGameMode::ALASTLASTTESTGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
