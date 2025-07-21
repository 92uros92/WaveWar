// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Player/ShadowCharacter.h"
#include "Player/ShadowPlayerState.h"
#include "Player/ShadowPlayerController.h"
#include "UI/WW_HUD.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "AbilitySystemComponent.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"




AShadowCharacter::AShadowCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f); // ...at this rotation rate

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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

}

void AShadowCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//	********************      Input      ****************************	//
//////////////////////////////////////////////////////////////////////////

void AShadowCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShadowCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShadowCharacter::Look);

		//Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AShadowCharacter::GunShoot);
	}
}

void AShadowCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AShadowCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AShadowCharacter::GunShoot()
{
	/** Montage for shooting */
	PlayAnimMontage(ShotMontage);

	/** After 0.2 second call GunShoot_TimerManager function */
	GetWorldTimerManager().SetTimer(ShotTimer, this, &AShadowCharacter::GunShoot_TimerManager, 0.2f);

}

void AShadowCharacter::GunShoot_TimerManager()
{
	/** Set location for spawning ammo */
	FVector GunLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform Transform = FTransform(GetControlRotation(), GunLocation);

	/** Spawn ammo */
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, Transform, SpawnParams);

}

//////////////////////////////////////////////////////////////////////////
//	********************      END Input      **************************	//
//////////////////////////////////////////////////////////////////////////

void AShadowCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Initialize ability for the Server
	InitAbilityActorInfo();
}

void AShadowCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Initialize ability for the Client
	InitAbilityActorInfo();
}

void AShadowCharacter::InitAbilityActorInfo()
{
	if (AShadowPlayerState* ShadowPlayerState = GetPlayerState<AShadowPlayerState>())
	{
		ShadowPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(ShadowPlayerState, this);
		AbilitySystemComponent = ShadowPlayerState->GetAbilitySystemComponent();
		AttributeSet = ShadowPlayerState->GetAttributeSet();

		AShadowPlayerController* ShadowPlayerController = Cast<AShadowPlayerController>(GetController());
		if (ShadowPlayerController)
		{
			AWW_HUD* WWHUD = Cast<AWW_HUD>(ShadowPlayerController->GetHUD());
			if (WWHUD)
			{
				WWHUD->InitScreenWidget(ShadowPlayerController, ShadowPlayerState, AbilitySystemComponent, AttributeSet);
			}
		}
	}
}
