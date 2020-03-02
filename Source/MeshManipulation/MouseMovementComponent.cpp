// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "MouseMovementComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "ProceduralShape.h"
#include "Engine/Engine.h"
#include "EngineGlobals.h"

#define printError(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Red,TEXT(text),false)
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Green,TEXT(text),false)



// Sets default values for this component's properties
UMouseMovementComponent::UMouseMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UMouseMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner() != NULL) {
		IsHolding = false;
		World = GetWorld();
		if (World != nullptr) {
			PC = World->GetFirstPlayerController();
		}
	}

}

// Called every frame
void UMouseMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsHolding) {
		float OldMouseX;
		if (PC) {
			OldMouseX = MouseXLocation;
			PC->GetMousePosition(MouseXLocation, MouseYLocation);
			float MouseXLocationDiff = MouseXLocation - OldMouseX;
			if (MouseXLocationDiff != 0) {
				OldMouseX = MouseXLocation;
				Shape->UpdateMesh(MouseXLocationDiff / 1000.f, MouseXLocationDiff / 1000.f);
			}
		}
	}
}

void UMouseMovementComponent::FindClickedVertex()
{
	FHitResult HitResult;
	if (PC != nullptr) {
		PC->GetMousePosition(MouseXLocation, MouseYLocation);
		FVector2D MousePosition = FVector2D(MouseXLocation, MouseYLocation);
		if (PC->GetHitResultAtScreenPosition(MousePosition, ECC_Visibility, false, HitResult)) {
			HitObject = HitResult.GetActor();
			if (HitObject != nullptr && HitObject->GetClass()->IsChildOf(AProceduralShape::StaticClass())) {
				Shape = static_cast<AProceduralShape*>(HitObject);

				if (Shape->GrabVertex(HitResult.ImpactPoint)) {
					print("Vertex grabbed");
					IsHolding = true;
				}
				else {
					printError("Too far from corner");
				}
			}
			else {
				printError("Nothing hit");
			}
		}
	}
}

void UMouseMovementComponent::StopManipulation()
{
	IsHolding = false;
}