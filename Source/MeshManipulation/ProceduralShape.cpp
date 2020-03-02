// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralShape.h"
#include "DrawDebugHelpers.h"
#include "ProceduralMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"

// Sets default values
AProceduralShape::AProceduralShape()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	CustomMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetRootComponent(CustomMesh);
}

// Called when the game starts or when spawned
void AProceduralShape::BeginPlay()
{
	Super::BeginPlay();
	GenerateCubeMesh();
}

bool AProceduralShape::GrabVertex(const FVector & ImpactPoint)
{
	FVector ClickedVertex = ImpactPoint - GetActorLocation();

	int32 VertexIndexMin = 0;
	float Distance = FVector::Dist(ClickedVertex, InteractiveVertices[0]);

	for (int32 i = 1; i < InteractiveVertices.Num(); i++) {
		float Temp = FVector::Dist(ClickedVertex, InteractiveVertices[i]);
		if (Temp <= GrabbingDistance && Temp < Distance) {
			Distance = Temp;
			VertexIndexMin = i;
		}
	}
	if (Distance <= GrabbingDistance) {
		/*Il vertice su cui bisogna effettuare l'operazione di scale
		è opposto al vertice cliccato*/
		GrabbedVertex = InteractiveVertices[(VertexIndexMin + 2) % InteractiveVertices.Num()];
		DebugVertex(InteractiveVertices[VertexIndexMin], FColor::Green);
		return true;
	}
	return false;
}

void AProceduralShape::GenerateCubeMesh()
{
	//These are relative locations to the placed Actor in the world
	Vertices.Add(FVector(0, -100, 0)); //lower left - 0
	Vertices.Add(FVector(0, -100, 100)); //upper left - 1
	Vertices.Add(FVector(0, 100, 0)); //lower right - 2 
	Vertices.Add(FVector(0, 100, 100)); //upper right - 3

	Vertices.Add(FVector(100, -100, 0)); //lower front left - 4
	Vertices.Add(FVector(100, -100, 100)); //upper front left - 5

	Vertices.Add(FVector(100, 100, 100)); //upper front right - 6
	Vertices.Add(FVector(100, 100, 0)); //lower front right - 7

	//Set Interactive vertices

	InteractiveVertices.Add(FVector(0, -100, 100));
	InteractiveVertices.Add(FVector(0, 100, 100));
	InteractiveVertices.Add(FVector(100, 100, 100));
	InteractiveVertices.Add(FVector(100, -100, 100));

	IndexInteractiveVertices.Add(1);
	IndexInteractiveVertices.Add(3);
	IndexInteractiveVertices.Add(6);
	IndexInteractiveVertices.Add(5);

	//Back face of cube
	AddTriangle(0, 2, 3);
	AddTriangle(3, 1, 0);

	//Left face of cube
	AddTriangle(0, 1, 4);
	AddTriangle(4, 1, 5);

	//Front face of cube
	AddTriangle(4, 5, 7);
	AddTriangle(7, 5, 6);

	//Right face of cube
	AddTriangle(7, 6, 3);
	AddTriangle(3, 2, 7);

	//Top face
	AddTriangle(1, 3, 5);
	AddTriangle(6, 5, 3);

	//bottom face
	AddTriangle(2, 0, 4);
	AddTriangle(4, 7, 2);

	TArray<FLinearColor> VertexColors;

	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);
	CustomMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void AProceduralShape::AddTriangle(float V1, float V2, float V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}

void AProceduralShape::UpdateMesh(float X, float Y)
{
	ShapeScale = FVector::OneVector + FVector(X, Y, 0.f);

	for (int32 index = 0; index < Vertices.Num(); index++)
	{
		Vertices[index] -= GrabbedVertex;
		Vertices[index] *= ShapeScale;
		Vertices[index] += GrabbedVertex;
	}
	for (int i = 0; i < InteractiveVertices.Num(); i++) {
		InteractiveVertices[i] = Vertices[IndexInteractiveVertices[i]];
	}

	CustomMesh->UpdateMeshSection(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
	CustomMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void AProceduralShape::DebugVertex(const FVector & position, FColor color)
{
	DrawDebugPoint(GetWorld(), position + GetActorLocation(), 5.0f, color, false, 8.0f);
}

