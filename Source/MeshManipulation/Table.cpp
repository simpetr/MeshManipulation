// Fill out your copyright notice in the Description page of Project Settings.


#include "Table.h"
#include "Components/StaticMeshComponent.h"
#include "SimpleCube.h"
#include "TableSurface.h"
#include "ChairLineManager.h"
#include "Engine/World.h"


// Sets default values
ATable::ATable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Transform");
	SetRootComponent(Mesh);

	ChairLineManagerComponent = CreateDefaultSubobject<UChairLineManager>(TEXT("ChairLineManager"));
	if (ChairLineManagerComponent == nullptr) {
		ChairLineManagerComponent->SetActive(true);
	}
}

// Called when the game starts or when spawned
void ATable::BeginPlay()
{
	Super::BeginPlay();
	World = GetWorld();
	if (World != nullptr) {
		ActorLocation = GetActorLocation();
		/*Creo Tavolo*/
		FVector SpawnLocation(ActorLocation.X, ActorLocation.Y, LegHeight);
		if (World != nullptr) {
			TableSurface = World->SpawnActor<ATableSurface>(ATableSurface::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
			TableSurface->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			TableSurface->GenerateMesh(TableSize, TableSize, TableHeight - LegHeight);

			int j = 0;
			float LegPosition = TableSize - LegSize;
			for (int i = 0; i < 4; i++) {
				if (i >= 2) j = 1;
				FVector LegLocation(ActorLocation.X + LegPosition * (i % 2), ActorLocation.Y + LegPosition * j, 0.f);
				ASimpleCube* Leg = World->SpawnActor<ASimpleCube>(ASimpleCube::StaticClass(), LegLocation, FRotator::ZeroRotator);
				Leg->GenerateMesh(LegSize, LegSize, LegHeight);
				TableLegs.Add(Leg);
			}

			Corners = TableSurface->GetCorners();
			for (int i = 0; i < 4; i++) {
				Corners[i] += ActorLocation;
			}
			ChairLineManagerComponent->ComponentInitialization();
		}
	}
}

void ATable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ChairLineManagerComponent->DestroyComponent();
	for (int i = 0; i < 4; i++)
		TableLegs[i]->Destroy();

}


/*Checks if I clicked on a vertex close to a corner*/
bool ATable::GrabVertex(const FVector & ClickPoint, FVector & OppositePoint)
{
	FVector LocalVector = ClickPoint - ActorLocation;
	Corners = TableSurface->GetCorners();
	for (int i = 0; i < 4; i++) {
		Corners[i].Z = TableHeight;
	}

	int VertexIndexMin = 0;
	float Distance = FVector::Dist(LocalVector, Corners[0]);

	for (int i = 1; i < 4; i++) {
		float Temp = FVector::Dist(LocalVector, Corners[i]);
		if (Temp <= GrabbingDistance && Temp < Distance) {
			Distance = Temp;
			VertexIndexMin = i;
		}
	}

	//I'm choosing the vertex opposite to the one I clicked
	if (Distance <= GrabbingDistance) {
		int index = (VertexIndexMin + 2) % Corners.Num();
		GrabbedVertex = Corners[index];
		OppositePoint = GrabbedVertex + ActorLocation;

		TableSurface->SetScalingPivot(GrabbedVertex);
		return true;
	}
	return false;
}

/*
*	In base al movimento del mouse, aggiorno le dimensioni del tavolo
*	ed il numero delle siede (e quindi la loro posizione)
*/
void ATable::UpdateTableSize(float X, float Y)
{
	TableSurface->UpdateProceduralMesh(X, Y);
	Corners = TableSurface->GetCorners();
	for (int i = 0; i < 4; i++) {
		Corners[i] += ActorLocation;
	}
	TableSize = FMath::Abs(Corners[1].X - Corners[0].X);
	UpdateLegsPosition();
	ChairLineManagerComponent->UpdatePosition();

}

void ATable::TranslateTable(float X, float Y)
{
	
	TableSurface->TranslateProceduralMesh(X, Y);

	Corners = TableSurface->GetCorners();
	for (int i = 0; i < 4; i++) {
		Corners[i] += ActorLocation;
	}
	UpdateLegsPosition();
	ChairLineManagerComponent->UpdatePosition();

}

void ATable::UpdateLegsPosition()
{
	TableLegs[0]->SetActorLocation(FVector(Corners[0].X, Corners[0].Y, 0.f));
	TableLegs[1]->SetActorLocation(FVector(Corners[1].X - LegSize, Corners[1].Y, 0.f));
	TableLegs[2]->SetActorLocation(FVector(Corners[2].X - LegSize, Corners[2].Y - LegSize, 0.f));
	TableLegs[3]->SetActorLocation(FVector(Corners[3].X, Corners[3].Y - LegSize, 0.f));
}

const TArray<FVector>& ATable::GetCorners()
{
	return Corners;
}

float ATable::GetTableSize()
{
	return TableSize;
}

float ATable::GetLegSize()
{
	return LegSize;
}
