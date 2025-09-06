// Uroš Pörš All Rights Reserved


#include "Player/ShadowCharacter.h"
#include "Player/ShadowPlayerState.h"
#include "Player/ShadowPlayerController.h"
#include "UI/WW_HUD.h"
#include "GAS/ShadowAbilitySystemComponent.h"
#include "../WaveWar.h"
#include "Data/LevelUpInfo.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundBase.h"





AShadowCharacter::AShadowCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetMesh()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 350.f;
	GetCharacterMovement()->AirControl = 0.35f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//SpringArm->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	SpringArm->SetupAttachment(RootComponent);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	/** By default character is not turning */
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

	/** CharacterClass for Shadow */
	CharacterClass = ECharacterClass::Shooter;
}

void AShadowCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AShadowCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime);

}

void AShadowCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Initialize ability for the Server
	InitAbilityActorInfo();

	/** Call function from CharacterBase class */
	AddCharacterAbilities();
}

void AShadowCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	/** Initialize ability for the Client */ 
	InitAbilityActorInfo();
}

int32 AShadowCharacter::GetPlayerLevel_Implementation()
{
	AShadowPlayerState* ShadowPlayerState = GetPlayerState<AShadowPlayerState>();
	check(ShadowPlayerState);

	/** Return Player level (it is on PlayerState). */
	return ShadowPlayerState->GetPlayerLevel();
}

void AShadowCharacter::AddToXP_Implementation(int32 InXP)
{
	AShadowPlayerState* ShadowPlayerState = GetPlayerState<AShadowPlayerState>();
	check(ShadowPlayerState);

	ShadowPlayerState->AddToXP(InXP);
}

void AShadowCharacter::LevelUp_Implementation()
{
	MulticastLevelUpSound();
}


void AShadowCharacter::MulticastLevelUpSound_Implementation()
{
	if (IsValid(LevelUpSound))
	{
		UGameplayStatics::PlaySound2D(this, LevelUpSound);
	}
}

int32 AShadowCharacter::GetXP_Implementation()
{
	AShadowPlayerState* ShadowPlayerState = GetPlayerState<AShadowPlayerState>();
	check(ShadowPlayerState);

	/** Return Player XP */
	return ShadowPlayerState->GetXP();
}

int32 AShadowCharacter::FindLevelForXP_Implementation(int32 InXP)
{
	AShadowPlayerState* ShadowPlayerState = GetPlayerState<AShadowPlayerState>();
	check(ShadowPlayerState);

	return ShadowPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

void AShadowCharacter::AddToPlayerLevel_Implementation(int32 InLevelToAdd)
{
	AShadowPlayerState* ShadowPlayerState = GetPlayerState<AShadowPlayerState>();
	check(ShadowPlayerState);

	ShadowPlayerState->AddToLevel(InLevelToAdd);
}

void AShadowCharacter::InitAbilityActorInfo()
{
	if (AShadowPlayerState* ShadowPlayerState = GetPlayerState<AShadowPlayerState>())
	{
		/** Initialize ShadowPlayerState, AbilitySystemComponent and AttributeSet */
		ShadowPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(ShadowPlayerState, this);
		Cast<UShadowAbilitySystemComponent>(ShadowPlayerState->GetAbilitySystemComponent())->SetAbilityActorInfo(); // Call SetAbilityActorInfo() from ShadowAbilitySystemComponent() class
		AbilitySystemComponent = ShadowPlayerState->GetAbilitySystemComponent();
		AttributeSet = ShadowPlayerState->GetAttributeSet();

		AShadowPlayerController* ShadowPlayerController = Cast<AShadowPlayerController>(GetController());
		if (ShadowPlayerController)
		{
			AWW_HUD* WWHUD = Cast<AWW_HUD>(ShadowPlayerController->GetHUD());
			if (WWHUD)
			{
				/** Initialize ScreenWidget */
				WWHUD->InitScreenWidget(ShadowPlayerController, ShadowPlayerState, AbilitySystemComponent, AttributeSet);
			}
		}
	}

	/** Because AbilitySystemComponent is valid, InitializeDefaultAttributes() function from CharacterBase can be called */
	InitializeDefaultAttributes();
}

/** Rotating upper body character 90 degrees at pitch and yaw axis as we are looking. Call in Tick function so we can update DeltaAimRotation. */
void AShadowCharacter::AimOffset(float DeltaTime)
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.0f;

	float Speed = Velocity.Size();

	bool bIsInAir = GetCharacterMovement()->IsFalling();

	/** Standing still and NOT jumping */
	if (Speed == 0.0f && !bIsInAir)
	{
		FRotator CurrentAimRotation = FRotator(0.0f, GetBaseAimRotation().Yaw, 0.0f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if (TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			NewAO_Yaw = AO_Yaw;
		}

		bUseControllerRotationYaw = true;

		/** Call TurnInPlace() here because when character not moving, only then can turn in place. */
		TurnInPlace(DeltaTime);
	}
	if (Speed > 0.0f || bIsInAir) // If we are moving or jumping then we don´t move upper body at yaw axis --> save start aiming position in StartingAimRotation 
	{
		StartingAimRotation = FRotator(0.0f, GetBaseAimRotation().Yaw, 0.0f);
		AO_Yaw = 0.0f;
		bUseControllerRotationYaw = true;

		/** Not turning because character is moving or jumping */
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.0f && !IsLocallyControlled())
	{
		/** Because server doesn´t get right value --> map pitch from [270, 360] to [-90, 0] */
		FVector2D InRange(270.0f, 360.0f);
		FVector2D OutRange(-90.0f, 0);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

/** Enum in TurnInPlace.h, set in ABP_Shadow */
void AShadowCharacter::TurnInPlace(float DeltaTime)
{
	if (AO_Yaw > 90.0f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right; // If character turn over 90°, turn right
	}
	else if (AO_Yaw < -90.0f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left; // If character turn under -90°, turn left
	}
	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		NewAO_Yaw = FMath::FInterpTo(NewAO_Yaw, 0.0f, DeltaTime, 10.0f); // Set new yaw
		AO_Yaw = NewAO_Yaw; 
		if (FMath::Abs(AO_Yaw) < 15.0f)
		{
			/** If character is not turning --> set StartingAimRotation */
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.0f, GetBaseAimRotation().Yaw, 0.0f);
		}
	}
}

FVector AShadowCharacter::GetCameraLocation()
{
	/** Return camera location of Character */
	return FollowCamera->GetComponentLocation();
}
