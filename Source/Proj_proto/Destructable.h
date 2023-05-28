// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Destructable.generated.h"

UCLASS()
class PROJ_PROTO_API ADestructable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructable();

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Particle")
		class UParticleSystem* Smash;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* Destroyed;

	void DestroyPot();

};
