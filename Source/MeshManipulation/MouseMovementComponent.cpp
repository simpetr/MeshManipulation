// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseMovementComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Table.h"
#include "TableSurface.h"
#include "Engine/Engine.h"
#include "EngineGlobals.h"
#include "DrawDebugHelpers.h"

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
	if (GetOwner() != nullptr) {
		IsHolding = false;
		World = GetWorld();
		if (World != nullptr) {
			PC = World->GetFirstPlayerController();
			SetComponentTickEnabled(false);
		}
	}

}


/* If the player is holding the left mouse button, the mouse movement will resize the table.
I still haven't found a better solution for managing the mouse movement related to the table*/
void UMouseMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsHolding) {
		if (PC) {
			FHitResult HitResult;
			float OldDistance;
			OldDistance = Distance;
			float MouseXDifference;
			float MouseYDifference;
			PC->GetInputMouseDelta(MouseXDifference, MouseYDifference);
			if (MouseXDifference != 0.f) {
				float MouseXLocation;
				float MouseYLocation;
				PC->GetMousePosition(MouseXLocation, MouseYLocation);
				FVector2D MousePosition = FVector2D(MouseXLocation, MouseYLocation);
				if (PC->GetHitResultAtScreenPosition(MousePosition, ECC_Visibility, false, HitResult)) {
					Distance = FVector::Dist(OppositePoint, HitResult.ImpactPoint);
					if (Distance > OldDistance) {
						if (MouseXDifference < 0.f)
							MouseXDifference *= -1.f;
					}
					if (Distance < OldDistance) {
						if (MouseXDifference > 0.f)
							MouseXDifference *= -1.f;
					}
					Table->ScaleTable(MouseXDifference / 50.f, MouseXDifference / 50.f);
				}
			}
		}
	}
}

void UMouseMovementComponent::FindClickedVertex()
{
	FHitResult HitResult;
	if (PC != nullptr) {
		float MouseXLocation;
		float MouseYLocation;
		PC->GetMousePosition(MouseXLocation, MouseYLocation);
		FVector2D MousePosition = FVector2D(MouseXLocation, MouseYLocation);
		if (PC->GetHitResultAtScreenPosition(MousePosition, ECC_Visibility, false, HitResult)) {
			HitObject = HitResult.GetActor();
			if (HitObject != nullptr && (dynamic_cast<ATableSurface*>(HitObject) != nullptr)) {
				if (HitObject->GetAttachParentActor() != nullptr) {
					Table = dynamic_cast<ATable*>(HitObject->GetAttachParentActor());
					if (Table->GrabVertex(HitResult.ImpactPoint, OppositePoint)) {
						print("Vertex grabbed");
						IsHolding = true;
						SetComponentTickEnabled(true);
					}
					else {
						printError("Too far from corner");
					}
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
	Distance = 0.f;
	SetComponentTickEnabled(false);
}

