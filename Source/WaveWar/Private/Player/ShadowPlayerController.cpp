// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Player/ShadowPlayerController.h"
#include "Interaction/EnemyInterface.h"
#include "Player/CharacterBase.h"
#include "GAS/ShadowAbilitySystemComponent.h"
#include "UI/FloatingDamageTextComp.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Components/InputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"




AShadowPlayerController::AShadowPlayerController()
{
	bReplicates = true;
}

void AShadowPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

//////////////////////////////////////////////////////////////////////////
//	********************      Input      ****************************	//
//////////////////////////////////////////////////////////////////////////

void AShadowPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	/** Set up action bindings */
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {

		/** Jumping */
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AShadowPlayerController::ShadowJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AShadowPlayerController::ShadowStopJumping);

		/** Moving */
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShadowPlayerController::Move);

		/** Looking */
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShadowPlayerController::Look);

		/** Shooting */
		//EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AShadowCharacter::GunShoot);

		/** Bind three callback functions (AbilityInputTagPressed, AbilityInputTagReleased and AbilityInputTagHeld) to InputAction */
		BindAbilityActions(InputConfig, this, &AShadowPlayerController::AbilityInputTagPressed, &AShadowPlayerController::AbilityInputTagReleased, &AShadowPlayerController::AbilityInputTagHeld);
	}
}

void AShadowPlayerController::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// add movement 
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AShadowPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// add yaw and pitch input to controller
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AShadowPlayerController::ShadowJump()
{
	GetCharacter()->Jump();
}

void AShadowPlayerController::ShadowStopJumping()
{
	GetCharacter()->StopJumping();
}

void AShadowPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Blue, *InputTag.ToString());
}

void AShadowPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetShadowASC() == nullptr)
		return;

	/** Call AbilityInputTagReleased function from UShadowAbilitySystemComponent class */
	GetShadowASC()->AbilityInputTagReleased(InputTag);
}

void AShadowPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetShadowASC() == nullptr)
		return;

	/** Call AbilityInputTagHeld function from UShadowAbilitySystemComponent class */
	GetShadowASC()->AbilityInputTagHeld(InputTag);
}

//////////////////////////////////////////////////////////////////////////
//	*******************      END Input      ***************************	//
//////////////////////////////////////////////////////////////////////////

UShadowAbilitySystemComponent* AShadowPlayerController::GetShadowASC()
{
	if (ShadowASC == nullptr)
	{
		ShadowASC = Cast<UShadowAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return ShadowASC;
}

void AShadowPlayerController::ShowFloatDamageNumber_Implementation(float Damage, ACharacter* TargetCharacter)
{
	if (IsValid(TargetCharacter) && FloatingDamageTextCompClass)
	{
		/** Create new component --> Manually register component because is not register in construct. */
		UFloatingDamageTextComp* DamageTextComp = NewObject<UFloatingDamageTextComp>(TargetCharacter, FloatingDamageTextCompClass);
		DamageTextComp->RegisterComponent();
		/** Attach to enemy when get hit and then detach from it. */
		DamageTextComp->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		/** Call function from UFloatingDamageTextComp. */
		DamageTextComp->SetFloatingDamageText(Damage);
	}
}



//void AShadowCharacter::GunShoot()
//{
//	/** Montage for shooting */
//	PlayAnimMontage(ShotMontage);
//
//	/** After 0.2 second call GunShoot_TimerManager function */
//	GetWorldTimerManager().SetTimer(ShotTimer, this, &AShadowCharacter::GunShoot_TimerManager, 0.2f);
//
//}
//
//void AShadowCharacter::GunShoot_TimerManager()
//{
//	/** Set location for spawning ammo */
//	FVector GunLocation = GetMesh()->GetSocketLocation("Muzzle_01");
//
//	FTransform Transform = FTransform(GetControlRotation(), GunLocation);
//
//	/** Spawn ammo */
//	FActorSpawnParameters SpawnParams;
//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	GetWorld()->SpawnActor<AActor>(ProjectileClass, Transform, SpawnParams);
//
//}
