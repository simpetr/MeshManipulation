// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MouseMovementComponent.generated.h"

class AProceduralShape;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MESHMANIPULATION_API UMouseMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMouseMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FindClickedVertex();
	void StopManipulation();
private:
	UWorld* World;
	APlayerController* PC;
	AActor* HitObject;
	AProceduralShape* Shape;
	bool IsHolding;
	float MouseXLocation, MouseYLocation;
		
};
