// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "UI/WW_WidgetController.h"
#include "ScreenWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);


UCLASS(BlueprintType, Blueprintable)
class WAVEWAR_API UScreenWidgetController : public UWW_WidgetController
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable, Category = "HealthAttributes")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "HealthAttributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	////****	FUNCTIONS	****////

	virtual void BroadcastInitialValues() override;
};
