// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "UI/WW_HUD.h"
#include "UI/WW_UserWidget.h"
#include "UI/ScreenWidgetController.h"




void AWW_HUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), ScreenWidgetClass);
	Widget->AddToViewport();

}

UScreenWidgetController* AWW_HUD::GetScreenWidgetController(const FWidgetControllerParams& WCParams)
{
	if (ScreenWidgetController == nullptr)
	{
		ScreenWidgetController = NewObject<UScreenWidgetController>(this, ScreenWidgetClass);
		ScreenWidgetController->SetWidgetControllerParams(WCParams);

		return ScreenWidgetController;
	}

	return ScreenWidgetController;
}

