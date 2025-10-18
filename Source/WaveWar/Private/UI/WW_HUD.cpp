// Uroš Pörš All Rights Reserved


#include "UI/WW_HUD.h"
#include "UI/WW_UserWidget.h"
#include "UI/ScreenWidgetController.h"
#include "UI/WW_WidgetController.h"
#include "UI/AttributeMenuWidgetController.h"
#include "Interaction/WidgetInterface.h"
#include "UI/MatchCountdownWidget.h"





void AWW_HUD::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

	if (!MatchCountdownWidget->Implements<UWidgetInterface>())
		return;

	IWidgetInterface::Execute_SetScreenTime(MatchCountdownWidget);
}

void AWW_HUD::InitScreenWidget(APlayerController* PCont, APlayerState* PSta, UAbilitySystemComponent* ASysCom, UAttributeSet* AttS)
{
	/** Don´t forget to set  ScreenWidgetClass and ScreenWidgetControllerClass in BP_WWHUD */
	check(ScreenWidgetClass);
	check(ScreenWidgetControllerClass);

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), ScreenWidgetClass);
	ScreenWidget = Cast<UWW_UserWidget>(Widget);
	MatchCountdownWidget = Cast<UMatchCountdownWidget>(ScreenWidget);

	/** Initialize FWidgetControllerParams */
	const FWidgetControllerParams WidgetControllerParams(PCont, PSta, ASysCom, AttS);

	/** Call GetScreenWidgetController() to set WidgetControllerParams */
	UScreenWidgetController* WidgetController = GetScreenWidgetController(WidgetControllerParams);

	/** Call SetWidgetController() from UWW_UserWidget */
	ScreenWidget->SetWidgetController(WidgetController);
	//MatchCountdownWidget->SetWidgetController(WidgetController);

	/** Broadcast to Health and MaxHealth */
	WidgetController->BroadcastInitialValues();

	Widget->AddToViewport();

	UWorld* World = GetWorld();
	if (World)
	{
		if (PCont)
		{
			FInputModeGameOnly InputModeData;
			PCont->SetInputMode(InputModeData);
		}
	}
}

UScreenWidgetController* AWW_HUD::GetScreenWidgetController(const FWidgetControllerParams& WCParams)
{
	if (ScreenWidgetController == nullptr)
	{
		/** If ScreenWidgetController is NULL then create one */
		ScreenWidgetController = NewObject<UScreenWidgetController>(this, ScreenWidgetControllerClass);
		ScreenWidgetController->SetWidgetControllerParams(WCParams);

		/** Whenever attributes changed then bind new value */
		ScreenWidgetController->InitBindingAttributes();
	}

	return ScreenWidgetController;
}

UAttributeMenuWidgetController* AWW_HUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		/** If AttributeMenuWidgetController is NULL then create one */
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);

		/** Whenever attributes changed then bind new value */
		AttributeMenuWidgetController->InitBindingAttributes();
	}

	return AttributeMenuWidgetController;
}

