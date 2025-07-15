// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WW_UserWidget.generated.h"




UCLASS()
class WAVEWAR_API UWW_UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	////****	FUNCTIONS	****////

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
