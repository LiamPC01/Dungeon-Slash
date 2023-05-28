// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingFood.h"

// Sets default values
AHealingFood::AHealingFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
}

// Called when the game starts or when spawned
void AHealingFood::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealingFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

