// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Sound/SoundBase.h"
#include "EnemyGoblin.generated.h"

UCLASS()
class PROJ_PROTO_API AEnemyGoblin : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyGoblin();

	UPROPERTY(BlueprintReadOnly)
		float Health = 100;
	UPROPERTY(BlueprintReadOnly)
		bool BIsAlive = true;
	UPROPERTY(BlueprintReadWrite)
		bool BIsBlocking = false;
	UPROPERTY(BlueprintReadOnly)
		bool BIsAttacking = false;
	UPROPERTY(BlueprintReadOnly)
		bool BIsHit = false;
	UPROPERTY(BlueprintReadOnly)
		float WalkSpeed = 0;
	UPROPERTY(BlueprintReadOnly)
		float AttackDmg = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool roamer = false;

	//UPROPERTY(VisibleAnywhere)
		//UStaticMeshComponent* LeftHand;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void HitReaction(float damage);
	void SpawnBlood();
	void SpawnSparks();

	void LineTraceForPlayer();
	

protected:

	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase* Block;
	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* Damage;
	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* Death;
	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* Miss;
	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* AttackWindUp;
	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* FallOverSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* TakeHit;
	UPROPERTY(EditAnywhere, Category = "Particle")
		class UParticleSystem* Blood;
	UPROPERTY(EditAnywhere, Category = "Particle")
		class UParticleSystem* Sparks;

	FTimerHandle ReactionTimerHandle;

	void EndReaction();
	bool GetBlocked();
	bool GetCanAction();
	void Attack();
	void EndAttack();
	void FallOver();
	

	//UCapsuleComponent* EnemyCapsule = GetCapsuleComponent();
	//UPrimitiveComponent* EnemyCapsulePrim = Cast<UPrimitiveComponent>(EnemyCapsule);

	FTimerHandle LineTraceTimerHandle;
	FTimerHandle AttackTimerHandle;
	FTimerHandle EndAttackTimerHandle;
	FTimerHandle FallOverTimerHandle;

	
};
