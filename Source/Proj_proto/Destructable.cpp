// Fill out your copyright notice in the Description page of Project Settings.

#include "kismet/GameplayStatics.h"
#include "Destructable.h"


// Sets default values
ADestructable::ADestructable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
}

// Called when the game starts or when spawned
void ADestructable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestructable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestructable::DestroyPot()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, Smash, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, Destroyed, GetActorLocation(), 2.f, 1.f, 0.f);
	Destroy();
}

