// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Player/ShadowCharacter.h"
#include "Player/ShadowPlayerState.h"
#include "Player/ShadowPlayerController.h"
#include "UI/WW_HUD.h"
#include "GAS/ShadowAbilitySystemComponent.h"

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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

}

void AShadowCharacter::BeginPlay()
{
	Super::BeginPlay();

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

int32 AShadowCharacter::GetPlayerLevel()
{
	AShadowPlayerState* ShadowPlayerState = GetPlayerState<AShadowPlayerState>();
	check(ShadowPlayerState);

	/** Return Player level (it is on PlayerState). */
	return ShadowPlayerState->GetPlayerLevel();
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
