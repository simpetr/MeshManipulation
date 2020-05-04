// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChairLine.generated.h"

class UStaticMeshComponent;
class AChair;
UCLASS()
class MESHMANIPULATION_API AChairLine : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AChairLine();
	void SetParameters(float ChairSizeV, float ChairLegHeightV, float ChairLegSizeV, float SeatHeightV, float BackSeatSizeV);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Chiamato prima che l'oggetto venga distrutto
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void AddChair(float Offset, float degreeY);
	void RemoveChair();
private:
	UStaticMeshComponent* Mesh;
	TArray<AChair*> ChairsStack;

	//Chair parameters
	float ChairSize;
	float ChairLegHeight;
	float ChairLegSize;
	float SeatHeight;
	float BackSeatSize;

};
