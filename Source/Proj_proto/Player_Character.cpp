// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Character.h"
#include "DrawDebugHelpers.h"
#include "UObject/UObjectBaseUtility.h"
#include "kismet/GameplayStatics.h"
#include "EnemyGoblin.h"
#include "HealingFood.h"
#include "Gold.h"
#include "Destructable.h"
#include "GameFramework/CharacterMovementComponent.h"




// Sets default values
APlayer_Character::APlayer_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MyCamera"));
	MyCamera->SetupAttachment(RootComponent);
	//MyCamera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));

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

	if (GetMovementComponent()->IsFalling() == false && GetVelocity().Size() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("velocity > 0"));
		if (BDoOnce)
		{
			GetWorldTimerManager().SetTimer(FootstepTimerHandle, this, &APlayer_Character::PlayFootstepSound, 0.5f, true, 0.0f);
			BDoOnce = false;
		}
		
	}
	else
	{
		BDoOnce = true;
		GetWorldTimerManager().ClearTimer(FootstepTimerHandle);
	}

	LineTraceForPickup();

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

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayer_Character::Attack);
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &APlayer_Character::Block);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayer_Character::Interact);
	PlayerInputComponent->BindAction("Consume", IE_Pressed, this, &APlayer_Character::Consume);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayer_Character::Pause);


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


void APlayer_Character::LineTraceForPawn()
{
	FHitResult OutHit;

	FVector ForwardVector = GetActorForwardVector();
	FVector Start = (ForwardVector * 100.f) + MyCamera->GetComponentLocation();
	FVector End = ((ForwardVector * 150.f) + Start);
	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = true;
	CollisionParams.AddIgnoredComponent(PlayerCapsulePrim);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Pawn, CollisionParams))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("The component being hit is %s"), *OutHit.GetComponent()->GetName()));
		BHitSuccess = true;

		// If OutHit is a goblin
		if (AEnemyGoblin* Goblin = Cast<AEnemyGoblin>(OutHit.GetActor()))
		{
			Goblin->HitReaction(AttackDmg);
		}
		else if (ADestructable* Destructable = Cast<ADestructable>(OutHit.GetActor()))
		{
			Destructable->DestroyPot();
		}

	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(this, Miss, GetActorLocation(), 0.75f, FMath::FRandRange(0.75f, 1.25f), 0.f);
	}

	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &APlayer_Character::EndAttack, 0.1f, false, 0.0f);

}

bool APlayer_Character::GetCanAction()
{
	return (BIsAlive && !BIsBlocking && !BIsAttacking && CurrentStamina >= AttackStaminaCost) ? true : false;
}

void APlayer_Character::TakeDamage(float value)
{
	UGameplayStatics::PlaySoundAtLocation(this, TakeDmg, GetActorLocation(), 1.f, 1.f, 0.f);
	CurrentHealth -= value;
	if (CurrentHealth <= 0)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "DeathLevel");
	}
}

void APlayer_Character::Attack()
{
	if (GetCanAction())
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
		BIsAttacking = true; // triggers animation
		CurrentStamina -= AttackStaminaCost;
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &APlayer_Character::LineTraceForPawn, 1.0f, false, 0.4f);
		GetWorldTimerManager().SetTimer(StaminaTimerHandle, this, &APlayer_Character::StaminaRegen, 0.05f, true, 2.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant Attack right now"));
	}

}

void APlayer_Character::EndAttack()
{
	BIsAttacking = false;
	UE_LOG(LogTemp, Warning, TEXT("AttackEnd"));
}

void APlayer_Character::Block()
{
	if (GetCanAction())
	{
		BIsBlocking = true;
		GetWorldTimerManager().SetTimer(BlockTimerHandle, this, &APlayer_Character::EndBlock, 1.0f, false, 0.7f);
		CurrentStamina -= BlockStaminaCost;
		GetWorldTimerManager().SetTimer(StaminaTimerHandle, this, &APlayer_Character::StaminaRegen, 0.05f, true, 2.0f);
	}
}

void APlayer_Character::EndBlock()
{
	BIsBlocking = false;
}

void APlayer_Character::StaminaRegen()
{
	if (CurrentStamina < MaxStamina)
		CurrentStamina += 4;
	else
	{
		GetWorldTimerManager().ClearTimer(StaminaTimerHandle);
	}
}

void APlayer_Character::Interact()
{

	FHitResult OutHit;

	FVector ForwardVector = GetActorForwardVector();
	FVector Start = (ForwardVector * 100.f) + MyCamera->GetComponentLocation();
	FVector End = ((ForwardVector * 250.f) + Start);
	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = true;
	CollisionParams.AddIgnoredComponent(PlayerCapsulePrim);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 5);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Pawn, CollisionParams))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("%s"), *OutHit.GetActor()->GetClass()->GetName()));

		// If OutHit is potion
		if (AHealingFood* Potion = Cast<AHealingFood>(OutHit.GetActor()))
		{
			numOfPotions++;
			Potion->Destroy();
			UGameplayStatics::PlaySoundAtLocation(this, PickUp, GetActorLocation(), 1.f, 1.f, 0.f);
		}
		else if (AGold* gold = Cast<AGold>(OutHit.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GOLD"));
			Gold += gold->GoldValue;
			gold->Destroy();
			UGameplayStatics::PlaySoundAtLocation(this, GoldPickUp, GetActorLocation(), 2.f, 1.f, 0.2f);

		}
		
		
	}
}

void APlayer_Character::Consume()
{
	if (numOfPotions > 0)
	{
		if (CurrentHealth >= MaxHealth)
		{
			UE_LOG(LogTemp, Warning, TEXT("health already full"));
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(this, Drink, GetActorLocation(), 1.f, 1.f, 0.f);
			CurrentHealth += 75;
			numOfPotions--;

			if (CurrentHealth > 100)
			{
				CurrentHealth -= (CurrentHealth - MaxHealth);
			}
		}
	}
}

void APlayer_Character::Pause()
{
	BIsPaused = !BIsPaused;
}

void APlayer_Character::PlayBlockSound()
{
	UGameplayStatics::PlaySoundAtLocation(this, BlockSound, GetActorLocation(), 1.f, FMath::FRandRange(0.9f, 1.1f), 0.f);
}

void APlayer_Character::PlayFootstepSound()
{
	UE_LOG(LogTemp, Warning, TEXT("footstep"));
	UGameplayStatics::PlaySoundAtLocation(this, Footsteps, GetActorLocation(), 0.2f, 1.f, 0.f);
}

void APlayer_Character::LineTraceForPickup()
{
	FHitResult OutHit;

	FVector ForwardVector = GetActorForwardVector();
	FVector Start = (ForwardVector * 100.f) + MyCamera->GetComponentLocation();
	FVector End = ((ForwardVector * 250.f) + Start);
	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = true;
	CollisionParams.AddIgnoredComponent(PlayerCapsulePrim);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1, 0, 5);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Pawn, CollisionParams))
	{
		BHitSuccess = true;

		// If OutHit is a potion
		if (AHealingFood* Food = Cast<AHealingFood>(OutHit.GetActor()))
		{
			PickupPrompt = "Press E to Pick up";
		}
		else if (AGold* gold = Cast<AGold>(OutHit.GetActor()))
		{
			PickupPrompt = "Press E to Pick up";
		}
		else
		{
			PickupPrompt = "";
		}

	}
	

}

void APlayer_Character::SpawnSparks()
{
	UE_LOG(LogTemp, Warning, TEXT("sparkssss"));
	SwordSocketLocation = GetMesh()->GetSocketLocation("SwordEnd");
		 
	UGameplayStatics::SpawnEmitterAtLocation(this, Sparks, SwordSocketLocation);
}






