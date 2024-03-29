// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gold.generated.h"

UCLASS()
class PROJ_PROTO_API AGold : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGold();
	UPROPERTY(EditAnywhere)
		float GoldValue = 10;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh;

};
