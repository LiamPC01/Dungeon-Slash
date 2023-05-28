// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGoblin.h"
#include "kismet/GameplayStatics.h"
#include <DrawDebugHelpers.h>
#include "Player_Character.h"


// Sets default values
AEnemyGoblin::AEnemyGoblin()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//LeftHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftHand"));

}

// Called when the game starts or when spawned
void AEnemyGoblin::BeginPlay()
{
	Super::BeginPlay();

	//line trace for player every x seconds to see if in range
	GetWorldTimerManager().SetTimer(LineTraceTimerHandle, this, &AEnemyGoblin::LineTraceForPlayer, FMath::FRandRange(2.f, 2.5f), true, 0.0f);

}

// Called every frame
void AEnemyGoblin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WalkSpeed = GetVelocity().Size();


}

// Called to bind functionality to input
void AEnemyGoblin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyGoblin::HitReaction(float damage)
{
	if (GetBlocked())
	{
		//Block event
		SpawnSparks();
		BIsBlocking = true;
		UGameplayStatics::PlaySoundAtLocation(this, Block, GetActorLocation(), 0.25f, FMath::FRandRange(0.75f, 1.25f), 0.f);
		GetWorldTimerManager().SetTimer(ReactionTimerHandle, this, &AEnemyGoblin::EndReaction, 1.f, false, 0.2f);
	}
	else
	{
		//Damage event
		//BIsHit = true; // trigger dmg anim
		SpawnBlood();
		Health -= damage;
		if (Health <= 0)
		{
			UGameplayStatics::PlaySoundAtLocation(this, Death, GetActorLocation(), 1.f, FMath::FRandRange(0.1f, 2.f), 0.5f);
			GetWorldTimerManager().SetTimer(FallOverTimerHandle, this, &AEnemyGoblin::FallOver, 1.f, false, 1.5f);
			BIsAlive = false;
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(this, TakeHit, GetActorLocation(), 1.5f, FMath::FRandRange(0.1f, 2.f), 0.1f);
		}
		UGameplayStatics::PlaySoundAtLocation(this, Damage, GetActorLocation(), 0.5f, FMath::FRandRange(0.75f, 1.25f), 0.f);
		GetWorldTimerManager().SetTimer(ReactionTimerHandle, this, &AEnemyGoblin::EndReaction, 1.f, false, 0.1f);
		
	}
}

void AEnemyGoblin::EndReaction()
{
	BIsBlocking = false;
	BIsHit = false;
}

bool AEnemyGoblin::GetBlocked()
{
	float BlockChance = 1;
	float BlockChanceMax = 4;

	if (FMath::RandRange(0.f, BlockChanceMax) <= BlockChance && GetCanAction())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AEnemyGoblin::GetCanAction()
{
	if (BIsAlive == true && !BIsBlocking && !BIsAttacking)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AEnemyGoblin::Attack()
{
	if (BIsAlive)
	{
		FHitResult OutHit;

		FVector ForwardVector = GetActorForwardVector();
		FVector Start = (ForwardVector * 100.f) + GetActorLocation();
		FVector End = ((ForwardVector * 50.f) + Start);
		FCollisionQueryParams CollisionParams;
		CollisionParams.bTraceComplex = true;
		//CollisionParams.AddIgnoredComponent(EnemyCapsulePrim);

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 5);

		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Pawn, CollisionParams))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("The component being hit is %s"), *OutHit.GetComponent()->GetName()));

			// If OutHit is player
			if (APlayer_Character* Player = Cast<APlayer_Character>(OutHit.GetActor()))
			{
				UE_LOG(LogTemp, Warning, TEXT("Playerhit"));
				if (Player->BIsBlocking == true)
				{
					Player->PlayBlockSound();
					Player->SpawnSparks();
					//goblin stun code here
				}
				else
				{
					Player->TakeDamage(AttackDmg);
				}
			}

		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(this, Miss, GetActorLocation(), 1.f, 1.f, 0.f);
		}

		GetWorldTimerManager().SetTimer(EndAttackTimerHandle, this, &AEnemyGoblin::EndAttack, 1.0f, false, 0.5f);

	}
	

}

void AEnemyGoblin::EndAttack()
{
	BIsAttacking = false;
}

void AEnemyGoblin::FallOver()
{
	UE_LOG(LogTemp, Warning, TEXT("fallover"));
	UGameplayStatics::PlaySoundAtLocation(this, FallOverSound, GetActorLocation(), 0.5f, 1.f, 0.f);
}

void AEnemyGoblin::SpawnBlood()
{
	UE_LOG(LogTemp, Warning, TEXT("spawn blood"));
	UGameplayStatics::SpawnEmitterAtLocation(this, Blood, GetActorLocation());
}

void AEnemyGoblin::SpawnSparks()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, Sparks, GetActorLocation());
}

void AEnemyGoblin::LineTraceForPlayer()
{
	if (GetCanAction())
	{


		FHitResult OutHit;

		FVector ForwardVector = GetActorForwardVector();
		FVector Start = (ForwardVector * 100.f) + GetActorLocation();
		FVector End = ((ForwardVector * 100.f) + Start);
		FCollisionQueryParams CollisionParams;
		CollisionParams.bTraceComplex = true;
		//CollisionParams.AddIgnoredComponent(EnemyCapsulePrim);

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 5);

		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Pawn, CollisionParams))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("The component being hit is %s"), *OutHit.GetComponent()->GetName()));
			//BHitSuccess = true;

			// If OutHit is player
			if (APlayer_Character* Player = Cast<APlayer_Character>(OutHit.GetActor()))
			{
				UE_LOG(LogTemp, Warning, TEXT("in range"));
				BIsAttacking = true;
				GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyGoblin::Attack, 1.0f, false, 0.7f);
				UGameplayStatics::PlaySoundAtLocation(this, AttackWindUp, GetActorLocation(), 2.5f, FMath::FRandRange(0.6f, 1.2f), 0.0f);

			}

		}
		else
		{
			//UGameplayStatics::PlaySoundAtLocation(this, Miss, GetActorLocation(), 1.f, 1.f, 0.f);
		}
	}

}

