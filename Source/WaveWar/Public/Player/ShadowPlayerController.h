// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Input/WW_InputConfig.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ShadowPlayerController.generated.h"






class IEnemyInterface;
struct FInputActionValue;
struct FInputActionInstance;
struct FGameplayTag;
class UShadowAbilitySystemComponent;
class UFloatingDamageTextComp;


UCLASS()
class WAVEWAR_API AShadowPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:

	////****	FUNCTIONS	****////

	AShadowPlayerController();

	/** APawn interface **/
	virtual void SetupInputComponent() override;

	/** Template to bind AbilityInputTagPressed, AbilityInputTagReleased and AbilityInputTagHeld functions */
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UWW_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);

	UFUNCTION(Client, Reliable)
	void ShowFloatDamageNumber(float Damage, ACharacter* TargetCharacter);

protected:

	////****	FUNCTIONS	****////

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UWW_InputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UShadowAbilitySystemComponent> ShadowASC;

	/** Variable to spawn UFloatingDamageTextComp class */
	UPROPERTY(EditDefaultsOnly, Category = "Damage Widget")
	TSubclassOf<UFloatingDamageTextComp> FloatingDamageTextCompClass;

	////****	FUNCTIONS	****////

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for jump and stop jumping input */
	void ShadowJump();
	void ShadowStopJumping();

	/** Callbacks for pressed, released and held functions */
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	/** Get AbilitySystemComponent */
	UShadowAbilitySystemComponent* GetShadowASC();

};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
inline void AShadowPlayerController::BindAbilityActions(const UWW_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent)
	{
		/** Loop through all Action in FWWInputAction, in UWW_InputConfig class */
		for (const FWWInputAction& Action : InputConfig->AbilityInputActions)
		{
			/** BindAction for pressed, released and held functions */
			if (Action.InputAction && Action.InputTag.IsValid())
			{
				if (PressedFunc)
				{
					EnhancedInputComponent->BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
				}
				if (ReleasedFunc)
				{
					EnhancedInputComponent->BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
				}
				if (HeldFunc)
				{
					EnhancedInputComponent->BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
				}
			}
		}
	}
}
