// Uroš Pörš All Rights Reserved


#include "UI/WW_UserWidget.h"





void UWW_UserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
