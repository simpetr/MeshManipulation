// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerWalkablePawn.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MouseMovementComponent.h"
#include "MouseClickEventsComponent.h"
#include "Engine/World.h"

// Sets default values
APlayerWalkablePawn::APlayerWalkablePawn()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 900.f; // The camera follows at this distance behind the character	
	CameraBoom->SocketOffset.Z = 300.f;
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;

	MouseComponent = CreateDefaultSubobject<UMouseMovementComponent>(TEXT("MouseResizeManager"));
	MouseClickComponent = CreateDefaultSubobject<UMouseClickEventsComponent>(TEXT("MouseClickManager"));

	if (MouseComponent == nullptr || MouseClickComponent == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("ERROR!!"));
	}
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void APlayerWalkablePawn::BeginPlay()
{
	Super::BeginPlay();
	IsHolding = false;
	World = GetWorld();
	if (World != nullptr) {
		PC = World->GetFirstPlayerController();
	}

}

void APlayerWalkablePawn::FindClickedVertex()
{
	if (MouseComponent)
		MouseComponent->FindClickedVertex();
	IsHolding = true;
}


void APlayerWalkablePawn::ReleasedClickedVertext()
{
	if (MouseComponent)
		MouseComponent->StopManipulation();
	IsHolding = false;
}


void APlayerWalkablePawn::SpawnTable()
{
	if (MouseClickComponent)
		MouseClickComponent->SpawnTable();
}

void APlayerWalkablePawn::TranslateTable()
{
	if (MouseClickComponent)
		MouseClickComponent->TranslateTable();
}


// Called to bind functionality to input
void APlayerWalkablePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Click", EInputEvent::IE_Pressed, this, &APlayerWalkablePawn::FindClickedVertex);
	PlayerInputComponent->BindAction("Click", EInputEvent::IE_Released, this, &APlayerWalkablePawn::ReleasedClickedVertext);
	PlayerInputComponent->BindAction("SpawnClick", EInputEvent::IE_Pressed, this, &APlayerWalkablePawn::SpawnTable);
	PlayerInputComponent->BindAction("Translate", EInputEvent::IE_Pressed, this, &APlayerWalkablePawn::TranslateTable);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerWalkablePawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerWalkablePawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerWalkablePawn::TurnAtRate);
	PlayerInputComponent->BindAxis("Look", this, &APlayerWalkablePawn::LookUpAtRate);



}

/*MOVEMENT AND ROTATION SECTION*/
void APlayerWalkablePawn::MoveForward(float Value)
{
	if (!IsHolding)
		if ((PC != NULL) && (Value != 0.0f))
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
}

void APlayerWalkablePawn::MoveRight(float Value)
{
	if (!IsHolding)
		if ((PC != NULL) && (Value != 0.0f))
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
}

void APlayerWalkablePawn::TurnAtRate(float Rate)
{
	if (!IsHolding)
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerWalkablePawn::LookUpAtRate(float Rate)
{
	if (!IsHolding)
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

/*END MOVEMENT SECTION*/