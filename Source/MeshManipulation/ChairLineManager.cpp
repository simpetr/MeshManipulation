// Fill out your copyright notice in the Description page of Project Settings.


#include "ChairLineManager.h"
#include "Table.h"
#include "ChairLine.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UChairLineManager::UChairLineManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UChairLineManager::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner() != nullptr) {
		Owner = static_cast<ATable*>(GetOwner());
		OwnerLocation = Owner->GetActorLocation();
		TableSize = Owner->GetTableSize();
		LegSize = Owner->GetLegSize();
	}
}
/*I'm using these anchor points to keep the chairs inline with the table when it got resized*/
void UChairLineManager::ComponentInitialization()
{
	/*Primo set delle anchors*/
	TArray<FVector> Corners = Owner->GetCorners();
	Spacing = LegSize + LegSpacing;
	AnchorsPosition.Add(FVector(Corners[0].X + Spacing, Corners[0].Y, 0.f));
	AnchorsPosition.Add(FVector(Corners[1].X, Corners[1].Y + Spacing, 0.f));
	AnchorsPosition.Add(FVector(Corners[2].X - Spacing, Corners[2].Y, 0.f));
	AnchorsPosition.Add(FVector(Corners[3].X, Corners[3].Y - Spacing, 0.f));
	/*Spawn Chairline*/
	if (GetWorld() != nullptr)
		for (int i = 0; i < AnchorsPosition.Num(); i++) {
			AChairLine* ChairLine = GetWorld()->SpawnActor<AChairLine>(AChairLine::StaticClass(), AnchorsPosition[i], FRotator(0.f, i*90.0f, 0.f));
			ChairLine->SetParameters(ChairSize, ChairLegHeight, ChairLegSize, SeatHeight, BackSeatSize);
			ChairLines.Add(ChairLine);
		}
	UpdateChairsCount();
}

void UChairLineManager::UpdatePosition()
{

	UpdateAnchorsPosition();
	UpdateChairsCount();
}

void UChairLineManager::UpdateAnchorsPosition()
{
	TArray<FVector> Corners = Owner->GetCorners();

	AnchorsPosition[0] = FVector(Corners[0].X + Spacing, Corners[0].Y, 0.f);
	AnchorsPosition[1] = FVector(Corners[1].X, Corners[1].Y + Spacing, 0.f);
	AnchorsPosition[2] = FVector(Corners[2].X - Spacing, Corners[2].Y, 0.f);
	AnchorsPosition[3] = FVector(Corners[3].X, Corners[3].Y - Spacing, 0.f);

	for (int i = 0; i < AnchorsPosition.Num(); i++) {
		ChairLines[i]->SetActorLocation(AnchorsPosition[i]);
	}
}
/*Check if there is space for a new chair*/
void UChairLineManager::UpdateChairsCount()
{
	TableSize = Owner->GetTableSize();
	float AbsoluteAvailableSpace = TableSize - LegSize * 2;
	float AvailableSpace = AbsoluteAvailableSpace - (ChairSize* ChairsNumber) - (LegSpacing * 2) - (ChairsSpacing * ChairsNumber);
	if (AvailableSpace > ChairSize) {
		//Add chair
		float Offset;
		if (ChairsNumber == 0)
			Offset = ChairSize;
		else Offset = ChairSize * (ChairsNumber + 1) + (ChairsSpacing * ChairsNumber);
		for (int i = 0; i < ChairLines.Num(); i++) {
			ChairLines[i]->AddChair(Offset, 180.f);
		}
		ChairsNumber++;
	}
	//Remove chair
	if (AvailableSpace < 0) {
		for (int i = 0; i < ChairLines.Num(); i++) {
			ChairLines[i]->RemoveChair();
		}
		ChairsNumber--;
	}
}

void UChairLineManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	for (int i = 0; i < ChairLines.Num(); i++) {
		ChairLines[i]->Destroy();
	}
}