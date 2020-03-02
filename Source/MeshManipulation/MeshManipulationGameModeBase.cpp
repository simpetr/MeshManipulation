// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "MeshManipulationGameModeBase.h"
#include "PlayerCharacter.h"
#include "CustomPlayerController.h"

AMeshManipulationGameModeBase::AMeshManipulationGameModeBase() {
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = ACustomPlayerController::StaticClass();
}