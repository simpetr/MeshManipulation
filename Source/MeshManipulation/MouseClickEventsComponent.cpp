// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseClickEventsComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "EngineGlobals.h"
#include "Table.h"
#include "TableSurface.h"
#include "DrawDebugHelpers.h"

#define printError(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Red,TEXT(text),false)
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Green,TEXT(text),false)

// Sets default values for this component's properties
UMouseClickEventsComponent::UMouseClickEventsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UMouseClickEventsComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner() != nullptr) {
		World = GetWorld();
		if (World != nullptr) {
			PC = World->GetFirstPlayerController();
		}
	}

}
/*If you click on a table you delete it, you spawn it otherwise.*/
void UMouseClickEventsComponent::SpawnTable()
{
	FHitResult HitResult;
	if (RaycastClick(HitResult)) {
		HitObject = HitResult.GetActor();
		if (HitObject != nullptr && (dynamic_cast<ATableSurface*>(HitObject) != nullptr)) {
			HitObject->Destroy();
			printError("Table destroyed!");
		}
		else {
			World->SpawnActor<ATable>(ATable::StaticClass(), HitResult.ImpactPoint, FRotator::ZeroRotator);
			print("Table spawned!");

		}
	}
}

void UMouseClickEventsComponent::TranslateTable()
{
	FHitResult HitResult;
	if (RaycastClick(HitResult)) {
		HitObject = HitResult.GetActor();
		if (IsClicked) {
			//already clicked on a table
			print("Endpoint hit");
			float TranslateX = HitResult.ImpactPoint.X - TableClickedPoint.X;
			float TranslateY = HitResult.ImpactPoint.Y - TableClickedPoint.Y;
			Table->TranslateTable(TranslateX, TranslateY);

			DrawDebugLine(
				GetWorld(),
				TableClickedPoint,
				HitResult.ImpactPoint,
				FColor::Red,
				false,
				3,
				0,
				5
			);

			IsClicked = false;
		}
		else {
			//need to click on the table
			if (HitObject != nullptr && (dynamic_cast<ATableSurface*>(HitObject) != nullptr)) {
				TableClickedPoint = HitResult.ImpactPoint;
				IsClicked = true;
				if (HitObject->GetAttachParentActor() != nullptr) {
					Table = dynamic_cast<ATable*>(HitObject->GetAttachParentActor());
				}
				print("Table hit");
			}
			else {
				//not hit the table
				printError("Nothing hit");

			}
		}
	}
}

bool UMouseClickEventsComponent::RaycastClick(FHitResult & HitResult)
{
	if (PC != nullptr) {
		float MouseXLocation;
		float MouseYLocation;
		PC->GetMousePosition(MouseXLocation, MouseYLocation);
		FVector2D MousePosition = FVector2D(MouseXLocation, MouseYLocation);
		if (PC->GetHitResultAtScreenPosition(MousePosition, ECC_Visibility, false, HitResult)) {
			return true;
		}
	}
	return false;
}