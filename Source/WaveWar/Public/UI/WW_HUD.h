// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WW_HUD.generated.h"



class UWW_UserWidget;
class UScreenWidgetController;
struct FWidgetControllerParams;


UCLASS()
class WAVEWAR_API AWW_HUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	TObjectPtr<UWW_UserWidget> ScreenWidget;

	////****	FUNCTIONS	****////

	UScreenWidgetController* GetScreenWidgetController(const FWidgetControllerParams& WCParams);

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWW_UserWidget> ScreenWidgetClass;

	UPROPERTY()
	TObjectPtr<UScreenWidgetController> ScreenWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UScreenWidgetController> ScreenWidgetControllerClass;
};
