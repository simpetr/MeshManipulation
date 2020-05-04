// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "MeshManipulationGameModeBase.h"
#include "PlayerWalkablePawn.h"
#include "CustomPlayerController.h"

AMeshManipulationGameModeBase::AMeshManipulationGameModeBase() {
	DefaultPawnClass = APlayerWalkablePawn::StaticClass();
	PlayerControllerClass = ACustomPlayerController::StaticClass();
}