// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WW_HUD.generated.h"



class UWW_UserWidget;
class UScreenWidgetController;
struct FWidgetControllerParams;
class UAbilitySystemComponent;
class UAttributeSet;
class UAttributeMenuWidgetController;


UCLASS()
class WAVEWAR_API AWW_HUD : public AHUD
{
	GENERATED_BODY()
	
public:


	////****	FUNCTIONS	****////

	UScreenWidgetController* GetScreenWidgetController(const FWidgetControllerParams& WCParams);

	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitScreenWidget(APlayerController* PCont, APlayerState* PSta, UAbilitySystemComponent* ASysCom, UAttributeSet* AttS);

private:

	UPROPERTY()
	TObjectPtr<UWW_UserWidget> ScreenWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWW_UserWidget> ScreenWidgetClass;

	UPROPERTY()
	TObjectPtr<UScreenWidgetController> ScreenWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UScreenWidgetController> ScreenWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
