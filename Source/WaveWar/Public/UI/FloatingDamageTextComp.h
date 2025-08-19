// Uro� P�r� All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FloatingDamageTextComp.generated.h"

/**
 * Component for WBP_FloatingDamage. Don�t forget set WidgetClass in BP_FloatingDamageText.
 */
UCLASS()
class WAVEWAR_API UFloatingDamageTextComp : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

	/** Implement function in BP_FloatingDamageText. */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFloatingDamageText(float Damage);
};
