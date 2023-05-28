// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player_Character.generated.h"

UCLASS()
class PROJ_PROTO_API APlayer_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer_Character();

	UPROPERTY(BlueprintReadOnly)
		bool BIsBlocking = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
		bool BIsAlive = true;
	UPROPERTY(BlueprintReadWrite)
		bool BIsAttacking = false;
	UPROPERTY(BlueprintReadOnly)
		float CurrentStamina = 100;
	UPROPERTY(BlueprintReadOnly)
		float MaxStamina = 100;
	UPROPERTY(BlueprintReadOnly)
		bool BHitSuccess = false;
	UPROPERTY(BlueprintReadOnly)
		float AttackDmg = 25;
	UPROPERTY(BlueprintReadOnly)
		float LastActionTime = 0;
	UPROPERTY(BlueprintReadOnly)
		float AttackStaminaCost = 10;
	UPROPERTY(BlueprintReadOnly)
		float BlockStaminaCost = 5;
	UPROPERTY(BlueprintReadOnly)
		float CurrentHealth = 100;
	UPROPERTY(BlueprintReadOnly)
		float MaxHealth = 100;
	UPROPERTY(BlueprintReadOnly)
		UGameplayStatics* GameStatics;

	UPROPERTY(BlueprintReadOnly)
		float numOfPotions = 0;

	UPROPERTY(BlueprintReadWrite)
		bool BIsPaused = false;
	UPROPERTY(BlueprintReadWrite)
		bool BDoOnce = true;
	UPROPERTY(BlueprintReadWrite)
		FString PickupPrompt = "";
	UPROPERTY(BlueprintReadOnly)
		float Gold = 0;
	UPROPERTY(BlueprintReadOnly)
	FVector SwordSocketLocation;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Value);
	void LookUpAtRate(float Value);	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float BaseLookUpAtRate;

	UPROPERTY(EditAnywhere)
		UCameraComponent* MyCamera;

	UFUNCTION(BlueprintCallable)
		void LineTraceForPawn();

	UFUNCTION(BlueprintCallable)
		bool GetCanAction();

	UFUNCTION(BlueprintCallable)
		void TakeDamage(float value);

	void Attack();
	void EndAttack();
	void Block();
	void EndBlock();
	void StaminaRegen();
	void Interact();
	void Consume();
	void Pause();
	void PlayBlockSound();
	void PlayFootstepSound();
	void LineTraceForPickup();
	void SpawnSparks();
	

private:

	// Casting own Capsule Component to primitive so it can be ignored by linetrace
	UCapsuleComponent* PlayerCapsule = GetCapsuleComponent();
	UPrimitiveComponent* PlayerCapsulePrim = Cast<UPrimitiveComponent>(PlayerCapsule);

	FTimerHandle AttackTimerHandle;
	FTimerHandle BlockTimerHandle;
	FTimerHandle StaminaTimerHandle;
	FTimerHandle FootstepTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase* Miss;
	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* TakeDmg;
	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* PickUp;
	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* Drink;
	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* BlockSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* Footsteps;
	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* GoldPickUp;
	UPROPERTY(EditAnywhere, Category = "Particle")
		class UParticleSystem* Sparks;
	

	

	



};
