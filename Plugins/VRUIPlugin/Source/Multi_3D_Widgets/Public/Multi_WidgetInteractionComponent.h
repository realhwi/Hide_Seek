// All Rights Reserved - 2024 - ReEngage Developments Ltd

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetInteractionComponent.h"
#include "Multi_WidgetInteractionComponent.generated.h"

/**
 * This class the Widget Interaction component that supports Z-Order interactions.
 * This must be used with the Multi Widget Component and the Multi User Widgets for Z-Order interactions to work.
 */
UCLASS(ClassGroup = "Multi_3D_Widgets", meta = (BlueprintSpawnableComponent, DisplayName = "Multi Widget Interaction"))
class MULTI_3D_WIDGETS_API UMulti_WidgetInteractionComponent : public UWidgetInteractionComponent
{
	GENERATED_BODY()
public:
	UMulti_WidgetInteractionComponent();
	virtual void PressPointerKey(FKey Key) override;

	// Statics
	static FKey queryKey;	// This is they key used to query if a userwidget will interact with a button or not.

protected:
	virtual FWidgetTraceResult PerformTrace() const override;

	UPROPERTY(EditAnywhere, Category = "Multi_3D_Widgets", meta = (tooltip = "This will output information about what this widget interactor is interacting with, for debugging purposes. (Disabled in shipping builds)"))
	bool bDebugInteractions = false;

	/* ---------------------------------- Advanced Collision ---------------------------------- */

	UPROPERTY(EditAnywhere, Category = "Multi_3D_Widgets", meta = (tooltip = "True to use a custom collision channel (to allow widget interactions through objects, see documentation)"))
	bool bUseCustomChannel = false;
	UPROPERTY(EditAnywhere, Category = "Multi_3D_Widgets", meta = (EditCondition = bUseCustomChannel, EditConditionHides))
	TEnumAsByte<ECollisionChannel> customChannel;
	UPROPERTY(EditAnywhere, Category = "Multi_3D_Widgets", meta = (EditCondition = bUseCustomChannel, EditConditionHides, tooltip = "If true, this widget will also interact with widgets that don't have the custom channel (but will prioritize widgets with the custom channel)."))
	bool bAlsoTestDefaultChannel = true;
};
