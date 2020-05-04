// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChairLineManager.generated.h"

class AChairLine;
class ATable;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MESHMANIPULATION_API UChairLineManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UChairLineManager();
	void ComponentInitialization();
	void UpdatePosition();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void UpdateAnchorsPosition();
	void UpdateChairsCount();

	TArray<AChairLine*> ChairLines;
	TArray<FVector> AnchorsPosition;
	ATable* Owner;
	FVector OwnerLocation;

	float TableSize;
	float LegSize;
	float ChairsSpacing = 10.f;
	float LegSpacing = 10.f;
	float Spacing;

	//Chair parameters
	float ChairSize = 150.0f;
	float ChairLegHeight = 80.f;
	float ChairLegSize = 20.f;
	float SeatHeight = 10.f;
	float BackSeatSize = 100.f;

	int ChairsNumber = 0;


};
