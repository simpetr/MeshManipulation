// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chair.generated.h"

class UStaticMeshComponent;
class ASimpleCube;
UCLASS()
class MESHMANIPULATION_API AChair : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChair();
	void GenerateChairMesh(float ChairSize, float ChairLegHeight, float ChairLegSize, float SeatHeight, float BackSeatSize);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UStaticMeshComponent * Mesh;
	TArray<ASimpleCube*> ChairParts;
};
