// Fill out your copyright notice in the Description page of Project Settings.


#include "Chair.h"
#include "Components/StaticMeshComponent.h"
#include "SimpleCube.h"

// Sets default values
AChair::AChair()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AChair::BeginPlay()
{
	Super::BeginPlay();
}

/*Create a chair using simple procedural cubes*/
void AChair::GenerateChairMesh(float ChairSize, float ChairLegHeight, float ChairLegSize, float SeatHeight, float BackSeatSize)
{
	/*Legs*/
	float Position = ChairSize - ChairLegSize;
	int j = 0;
	for (int i = 0; i < 4; i++) {
		ASimpleCube* ChairPart = GetWorld()->SpawnActor<ASimpleCube>(ASimpleCube::StaticClass());
		ChairPart->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		ChairPart->GenerateMesh(ChairLegSize, ChairLegSize, ChairLegHeight);
		if (i >= 2) j = 1;
		ChairPart->SetActorRelativeLocation(FVector(Position*(i % 2), Position*j, 0.f));
		ChairParts.Add(ChairPart);
	}
	/*Seat and backseat*/
	ASimpleCube* SeatPart = GetWorld()->SpawnActor<ASimpleCube>(ASimpleCube::StaticClass());
	SeatPart->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	SeatPart->GenerateMesh(ChairSize, ChairSize - ChairLegSize, SeatHeight);
	SeatPart->SetActorRelativeLocation(FVector(0, 0, ChairLegHeight));
	ChairParts.Add(SeatPart);

	ASimpleCube* BackSeatPart = GetWorld()->SpawnActor<ASimpleCube>(ASimpleCube::StaticClass());
	BackSeatPart->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	BackSeatPart->GenerateMesh(ChairSize, ChairLegSize, BackSeatSize);
	BackSeatPart->SetActorRelativeLocation(FVector(0, ChairSize - ChairLegSize, ChairLegHeight));
	ChairParts.Add(BackSeatPart);
}

void AChair::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	for (int i = 0; i < ChairParts.Num(); i++) {
		ChairParts[i]->Destroy();
	}
}
