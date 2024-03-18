// All Rights Reserved - 2024 - ReEngage Developments Ltd

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Multi_DisplayerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = "Multi_3D_Widgets", meta = (BlueprintSpawnableComponent, DisplayName = "Multi Displayer Component"))
class MULTI_3D_WIDGETS_API UMulti_DisplayerComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UMulti_DisplayerComponent();

	UFUNCTION(BlueprintPure, Category = "Multi_3D_Widgets")
	class AMulti_Displayer* getDisplayer() { return displayer; };

protected:
	virtual void BeginPlay() override;
	void spawnMulti_Displayer();

	class AMulti_Displayer* displayer = 0;

	/* ---------------------------------- Advanced Collision ---------------------------------- */

	UPROPERTY(EditAnywhere, Category = "Multi_3D_Widgets", meta = (tooltip = "True to use a custom collision channel (to allow widget interactions through objects, see documentation)"))
	bool bUseCustomChannel = false;
	UPROPERTY(EditAnywhere, Category = "Multi_3D_Widgets", meta = (EditCondition = bUseCustomChannel, EditConditionHides))
	TEnumAsByte<ECollisionChannel> customChannel;
};
