// Uroš Pörš All Rights Reserved


#include "Input/WW_InputConfig.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"




const UInputAction* UWW_InputConfig::FindAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	/** Loop through AbilityInputActions array, return InputAction if found */
	for (const FWWInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can´t find AbilityInputAction for InputTag (%s), on InputConfig (%s)"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
