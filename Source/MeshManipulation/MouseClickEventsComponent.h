// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MouseClickEventsComponent.generated.h"

class ATable;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MESHMANIPULATION_API UMouseClickEventsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMouseClickEventsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/*Permette di creare o eliminare un tavolo*/
	void SpawnTable();

private:
	UWorld* World;
	APlayerController* PC;
	AActor* HitObject;
	ATable* Table;

};
