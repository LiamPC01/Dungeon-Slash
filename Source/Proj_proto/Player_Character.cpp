// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Character.h"

// Sets default values
APlayer_Character::APlayer_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayer_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayer_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayer_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayer_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayer_Character::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APlayer_Character::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayer_Character::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayer_Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayer_Character::LookUpAtRate);

}

void APlayer_Character::MoveForward(float Value)
{
	if ((Controller) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	
}

void APlayer_Character::MoveRight(float Value)
{
	if ((Controller) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void APlayer_Character::TurnAtRate(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayer_Character::LookUpAtRate(float Value)
{
	AddControllerPitchInput(Value * BaseLookUpAtRate * GetWorld()->GetDeltaSeconds());
}

