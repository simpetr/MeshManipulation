// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleCube.generated.h"

class UProceduralMeshComponent;
UCLASS()
class MESHMANIPULATION_API ASimpleCube : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASimpleCube();
	void GenerateMesh(float X, float Y, float Z);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void AddTriangle(int32 V1, int32 V2, int32 V3);
	UProceduralMeshComponent* CustomMesh;
	TArray<FVector> Vertices;
	TArray<int32> Triangles;

};
