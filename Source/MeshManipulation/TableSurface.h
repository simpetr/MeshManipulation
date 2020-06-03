// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TableSurface.generated.h"

class UProceduralMeshComponent;
UCLASS()
class MESHMANIPULATION_API ATableSurface : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATableSurface();
	void GenerateMesh(float X, float Y, float Z);
	TArray<FVector>& GetCorners();
	void SetScalingPivot(const FVector& Pivot);
	void UpdateProceduralMesh(float X, float Y);
	void TranslateProceduralMesh(float X, float Y);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void AddTriangle(int32 V1, int32 V2, int32 V3);
	UProceduralMeshComponent* CustomMesh;
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Corners;
	FVector ScalingPivot;

};
