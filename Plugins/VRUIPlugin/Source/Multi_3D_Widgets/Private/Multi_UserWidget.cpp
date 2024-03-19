// All Rights Reserved - 2024 - ReEngage Developments Ltd

#include "Multi_UserWidget.h"
#include "Multi_WidgetInteractionComponent.h"

void UMulti_UserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == UMulti_WidgetInteractionComponent::queryKey)
	{
		bQueryWidget = true;
	}

	return Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UMulti_UserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == UMulti_WidgetInteractionComponent::queryKey)
	{
		bQueryWidget = false;
	}

	Super::NativeOnMouseLeave(InMouseEvent);
}
