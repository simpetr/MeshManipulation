// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Table.generated.h"

class UStaticMeshComponent;
class ASimpleCube;
class UChairLineManager;
class ATableSurface;
UCLASS()
class MESHMANIPULATION_API ATable : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATable();
	bool GrabVertex(const FVector& ClickPoint, FVector& OppositePoint);
	void UpdateTableSize(float X, float Y);
	void TranslateTable(float X, float Y);
	const TArray<FVector>& GetCorners();
	float GetTableSize();
	float GetLegSize();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void UpdateLegsPosition();

	UStaticMeshComponent * Mesh;
	UChairLineManager* ChairLineManagerComponent;
	ATableSurface* TableSurface;
	TArray<ASimpleCube*> TableLegs;
	TArray<FVector> Corners;
	UWorld* World;
	FVector ActorLocation;

	/*Table dimensions*/
	float TableHeight = 190.f;
	float TableSize = 240.f;
	float LegHeight = 170.f;
	float LegSize = 20.f;

	float GrabbingDistance = 50.f;
	FVector GrabbedVertex;


};

