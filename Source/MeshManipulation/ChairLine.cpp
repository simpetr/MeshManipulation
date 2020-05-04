// Fill out your copyright notice in the Description page of Project Settings.


#include "ChairLine.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Chair.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AChairLine::AChairLine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Transform");
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AChairLine::BeginPlay()
{
	Super::BeginPlay();
}


void AChairLine::AddChair(float Offset, float degreeY)
{
	FVector SpawnVector = FVector(Offset, 0.f, 0.f);
	AChair* Chair = GetWorld()->SpawnActor<AChair>(AChair::StaticClass());
	Chair->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	Chair->SetActorRelativeLocation(SpawnVector);
	Chair->SetActorRotation(GetActorRotation() + FRotator(0, degreeY, 0));
	Chair->GenerateChairMesh(ChairSize, ChairLegHeight, ChairLegSize, SeatHeight, BackSeatSize);
	ChairsStack.Push(Chair);
}

void AChairLine::RemoveChair()
{
	ChairsStack.Pop()->Destroy();
}

void AChairLine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	for (int32 i = 0; i < ChairsStack.Num(); i++) {
		ChairsStack[i]->Destroy();
	}
}

void AChairLine::SetParameters(float ChairSizeV, float ChairLegHeightV, float ChairLegSizeV, float SeatHeightV, float BackSeatSizeV)
{
	this->ChairSize = ChairSizeV;
	this->ChairLegHeight = ChairLegHeightV;
	this->ChairLegSize = ChairLegSizeV;
	this->SeatHeight = SeatHeightV;
	this->BackSeatSize = BackSeatSizeV;
}

