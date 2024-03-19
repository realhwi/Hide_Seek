// All Rights Reserved - 2024 - ReEngage Developments Ltd

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Multi_Displayer.generated.h"

UCLASS()
class MULTI_3D_WIDGETS_API AMulti_Displayer : public AActor
{
	GENERATED_BODY()
public:
	struct FZOrderStruct
	{
		FZOrderStruct() { }
		FZOrderStruct(int32 initZOrder) { zOrder = initZOrder; };
		bool operator< (const FZOrderStruct& other) const
		{
			return zOrder < other.zOrder;
		}

		int32 zOrder = -1;
		TArray<class UMulti_WidgetComponent*> widgets;
	};

public:
	AMulti_Displayer();
	void applyZOrder();
	void setCustomChannel(ECollisionChannel newCustomChannel);
	void setDontUseCustomChannel();

	UFUNCTION(BlueprintCallable, Category = "Multi_3D_Widgets", meta = (AdvancedDisplay = "zOrder", DisplayName = "Add UI To Displayer"))
	UUserWidget* addUI(TSubclassOf<class UUserWidget> widgetClass, int32 zOrder = 0);

	// Getters
	class UMulti_WidgetComponent* getWidgetComponentTemplate() { return widgetComponentTemplate; };

protected:
	virtual void BeginPlay() override;
	void addtoZOrderStruct(class UMulti_WidgetComponent* widget, int32 zOrder);

	/* ---------------------------------- Components ---------------------------------- */

	// This is the template widget. It is not meant to display any UI, but serves to allow designers to easily change how the UI will look in game.
	UPROPERTY(EditAnywhere, Category = "Defaults")
	class UMulti_WidgetComponent* widgetComponentTemplate;
	TArray<FZOrderStruct> widgets;

	/* ---------------------------------- Advanced Collision ---------------------------------- */

	UPROPERTY(EditAnywhere, Category = "Multi_3D_Widgets", meta = (tooltip = "True to use a custom collision channel (to allow widget interactions through objects, see documentation)"))
	bool bUseCustomChannel = false;
	UPROPERTY(EditAnywhere, Category = "Multi_3D_Widgets", meta = (EditCondition = bUseCustomChannel, EditConditionHides))
	TEnumAsByte<ECollisionChannel> customChannel;
};
