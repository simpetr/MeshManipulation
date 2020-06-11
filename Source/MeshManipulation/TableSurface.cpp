// Fill out your copyright notice in the Description page of Project Settings.


#include "TableSurface.h"
#include "ProceduralMeshComponent.h"	
#include "DrawDebugHelpers.h"

// Sets default values
ATableSurface::ATableSurface()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralTableSurfaceMesh");
	CustomMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetRootComponent(CustomMesh);

}

// Called when the game starts or when spawned
void ATableSurface::BeginPlay()
{
	Super::BeginPlay();
	Corners.Init(FVector(0.f, 0.f, 0.f), 4);

}

void ATableSurface::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	this->GetAttachParentActor()->Destroy();
}


void ATableSurface::GenerateMesh(float X, float Y, float Z)
{
	Vertices.Add(FVector(0.f, 0.f, 0.f));
	Vertices.Add(FVector(X, 0.f, 0.f));
	Vertices.Add(FVector(X, Y, 0.f));
	Vertices.Add(FVector(0.f, Y, 0.f));
	Vertices.Add(FVector(0.f, 0.f, Z));
	Vertices.Add(FVector(X, 0.f, Z));
	Vertices.Add(FVector(X, Y, Z));
	Vertices.Add(FVector(0.f, Y, Z));

	//Bottom face
	AddTriangle(0, 1, 3);
	AddTriangle(1, 2, 3);

	//Top face
	AddTriangle(5, 4, 7);
	AddTriangle(5, 7, 6);

	//Forward Face 
	AddTriangle(5, 2, 1);
	AddTriangle(5, 6, 2);

	//Backward Face
	AddTriangle(4, 0, 3);
	AddTriangle(4, 3, 7);

	////Right Face
	AddTriangle(7, 3, 2);
	AddTriangle(7, 2, 6);

	////Left Face
	AddTriangle(4, 5, 1);
	AddTriangle(4, 1, 0);

	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	CustomMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

/*Scales the mesh around a certain pivot*/
void ATableSurface::UpdateProceduralMesh(float X, float Y)
{
	FVector ShapeScale = FVector::OneVector + FVector(X, Y, 0.f);
	for (int32 index = 0; index < Vertices.Num(); index++)
	{
		Vertices[index] -= ScalingPivot;
		Vertices[index] *= ShapeScale;
		Vertices[index] += ScalingPivot;
	}

	CustomMesh->UpdateMeshSection(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
	CustomMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

}
void ATableSurface::TranslateProceduralMesh(float X, float Y)
{
	for (int32 index = 0; index < Vertices.Num(); index++)
	{
		Vertices[index] += FVector(X, Y, 0);
	}
	CustomMesh->UpdateMeshSection(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
	CustomMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

/*Return the top verticies of the table*/
TArray<FVector>& ATableSurface::GetCorners()
{
	for (int i = 0; i < 4; i++) {
		Corners[i] = Vertices[i + 4];
	}
	return Corners;
}

void ATableSurface::SetScalingPivot(const FVector & Pivot)
{
	ScalingPivot = Pivot;
}

void ATableSurface::AddTriangle(int32 V1, int32 V2, int32 V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}

/*Maybe future update*/
/*void ATableSurface::RotateProceduralMesh(float Angle, FQuat & Rota)
{
	float Xce = (Corners[0].X + Corners[2].X) / 2;
	float Yce = (Corners[0].Y + Corners[2].Y) / 2;
	FVector center = FVector(Xce, Yce, 0);
	FRotator EuRotator(0, Angle, 0);
	Rota = EuRotator.Quaternion();
	for (int32 index = 0; index < Vertices.Num(); index++)
	{
		Vertices[index] = Rota * (Vertices[index] - center) + center;
	}
	CustomMesh->UpdateMeshSection(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
	CustomMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

}*/
