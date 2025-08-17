// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FloatingDamageTextComp.generated.h"

/**
 * Component for WBP_FloatingDamage. Don´t forget set WidgetClass in BP_FloatingDamageText.
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
