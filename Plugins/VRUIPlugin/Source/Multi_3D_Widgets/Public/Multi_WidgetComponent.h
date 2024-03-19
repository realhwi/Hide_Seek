// All Rights Reserved - 2024 - ReEngage Developments Ltd

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Multi_WidgetComponent.generated.h"

/**
 * This is a Widget Component that supports Z-Order widget interactions.
 */
UCLASS(ClassGroup = "Multi_3D_Widgets", meta = (BlueprintSpawnableComponent, DisplayName = "Multi Widget Component"))
class MULTI_3D_WIDGETS_API UMulti_WidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	virtual void SetWidget(UUserWidget* InWidget) override;
	void copyProperties(const UWidgetComponent* copyFrom);
	void setCustomChannel(ECollisionChannel newCustomChannel);
	void setDontUseCustomChannel();

protected:

	UFUNCTION()
	void OnWidgetVisibilityChanged(ESlateVisibility InVisibility);

	bool bUseCustomChannel = false;
	ECollisionChannel customChannel;
};
