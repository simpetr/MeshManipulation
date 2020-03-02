// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "ProceduralShape.generated.h"

class UProceduralMeshComponent;
UCLASS()
class MESHMANIPULATION_API AProceduralShape : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralShape();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	bool GrabVertex(const FVector& ImpactPoint);
	void UpdateMesh(float X, float Y);
private:
	
	UProceduralMeshComponent* CustomMesh;
	
	TArray<FVector> Vertices;
	
	TArray<int32> Triangles;
	
	TArray<FVector> InteractiveVertices;
	TArray<int32> IndexInteractiveVertices;

	FVector ShapeScale;
	FVector GrabbedVertex;

	float GrabbingDistance = 30.f;

	void GenerateCubeMesh();
	void AddTriangle(float V1, float V2, float V3);
	
	void DebugVertex(const FVector& position, FColor color = FColor(255, 0, 255));

};
