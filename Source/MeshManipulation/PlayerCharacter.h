// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UMouseMovementComponent;
class UCameraComponent;
UCLASS()
class MESHMANIPULATION_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	UMouseMovementComponent* MouseComponent;
	UWorld* World;
	APlayerController* PC;
	AActor* HitObject;
	bool IsHolding;

	/** Camera boom positioning the camera behind the character */
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UCameraComponent* FollowCamera;

	void FindClickedVertex();
	
	void ReleasedClickedVertext();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	float BaseLookUpRate;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

};
