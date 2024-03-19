// All Rights Reserved - 2024 - ReEngage Developments Ltd

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Multi_UserWidget.generated.h"

/**
 * This is a UserWidget class that supports Z-Order widget interactions in 3D.
 * This must be used with the Multi Widget Component and the Multi Widget Interaction Component for Z-Order interactions to work. 
 */
UCLASS()
class MULTI_3D_WIDGETS_API UMulti_UserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	bool isQueryWidget() { return bQueryWidget; };

protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	bool bQueryWidget = false;
};
