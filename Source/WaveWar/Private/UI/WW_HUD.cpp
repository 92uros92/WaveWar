// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "UI/WW_HUD.h"
#include "UI/WW_UserWidget.h"
#include "UI/ScreenWidgetController.h"
#include "UI/WW_WidgetController.h"





void AWW_HUD::InitScreenWidget(APlayerController* PCont, APlayerState* PSta, UAbilitySystemComponent* ASysCom, UAttributeSet* AttS)
{
	/** Don´t forget to set  ScreenWidgetClass and ScreenWidgetControllerClass in BP_WWHUD */
	check(ScreenWidgetClass);
	check(ScreenWidgetControllerClass);

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), ScreenWidgetClass);
	ScreenWidget = Cast<UWW_UserWidget>(Widget);

	/** Initialize FWidgetControllerParams */
	const FWidgetControllerParams WidgetControllerParams(PCont, PSta, ASysCom, AttS);

	/** Call GetScreenWidgetController() to set WidgetControllerParams */
	UScreenWidgetController* WidgetController = GetScreenWidgetController(WidgetControllerParams);

	/** Call SetWidgetController() from UWW_UserWidget */
	ScreenWidget->SetWidgetController(WidgetController);

	/** Broadcast to Health and MaxHealth */
	WidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}

UScreenWidgetController* AWW_HUD::GetScreenWidgetController(const FWidgetControllerParams& WCParams)
{
	if (ScreenWidgetController == nullptr)
	{
		ScreenWidgetController = NewObject<UScreenWidgetController>(this, ScreenWidgetControllerClass);
		ScreenWidgetController->SetWidgetControllerParams(WCParams);

		return ScreenWidgetController;
	}

	return ScreenWidgetController;
}

