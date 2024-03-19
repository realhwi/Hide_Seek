// All Rights Reserved - 2023 - ReEngage Developments Ltd

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VR_UI_HUD.generated.h"

/**
 * This is the VR UI HUD
 * It automatically detects if playing in VR or not, and either displays UserWidgets to the viewport or to VR UI Displayer(s)
 */
UCLASS()
class VR_UI_API AVR_UI_HUD : public AHUD
{
	GENERATED_BODY()
protected:
	struct FWidgetInfo
	{
		TSubclassOf<class UUserWidget> widgetClass;
		int32 zOrder = 0;
	};

public:
	// Creates UI and, if in VR, adds to it a VR_UI_Displayer already spawned in a level 
	UFUNCTION(BlueprintCallable, Category = "VR UI", meta = (DeterminesOutputType = "widgetClass", AdvancedDisplay = "zOrder", DisplayName = "Create UI (On a Displayer in the Level)"))
	class UUserWidget* createUIOnDisplayerInLevel(TSubclassOf<class UUserWidget> widgetClass, int32 zOrder = 0);
	// Creates UI and, if in VR, adds to it a VR_UI_Displayer already spawned in a level.
	// "Diff" So this uses different UserWidgets if in VR or Flat
	UFUNCTION(BlueprintCallable, Category = "VR UI", meta = (DeterminesOutputType = "flatWidgetClass", AdvancedDisplay = "zOrder", DisplayName = "Create UI Diff (On a Displayer in the Level)"))
	class UUserWidget* createUIOnDisplayerInLevel_Diff(UPARAM(DisplayName = "Flat Widget Class")TSubclassOf<class UUserWidget> flatWidgetClass, UPARAM(DisplayName="VR Widget Class")TSubclassOf<class UUserWidget> VRWidgetClass, int32 zOrder = 0);

	// Create UI and, if in VR, adds it to the specified VR_UI_Displayer
	UFUNCTION(BlueprintCallable, Category = "VR UI", meta = (DeterminesOutputType = "widgetClass", AdvancedDisplay = "zOrder", DisplayName = "Create UI (On a Specified Displayer)"))
	class UUserWidget* createUIOnSpecificDisplayer(TSubclassOf<class UUserWidget> widgetClass, class AMulti_Displayer *displayer, int32 zOrder = 0);
	// Create UI and, if in VR, adds it to the specified VR_UI_Displayer
	// "Diff" So this uses different UserWidgets if in VR or Flat
	UFUNCTION(BlueprintCallable, Category = "VR UI", meta = (DeterminesOutputType = "flatWidgetClass", AdvancedDisplay = "zOrder", DisplayName = "Create UI Diff (On a Specified Displayer)"))
	class UUserWidget* createUIOnSpecificDisplayer_Diff(UPARAM(DisplayName = "Flat Widget Class")TSubclassOf<class UUserWidget> flatWidgetClass, UPARAM(DisplayName = "VR Widget Class")TSubclassOf<class UUserWidget> VRWidgetClass, class AMulti_Displayer *displayer, int32 zOrder = 0);

	// Create UI and, if in VR, adds it to the VR UI Displayer Component.
	UFUNCTION(BlueprintCallable, Category = "VR UI", meta = (DeterminesOutputType = "widgetClass", AdvancedDisplay = "zOrder", DisplayName = "Create UI (On a pawn's VR UI Displayer Component)"))
	class UUserWidget* createUIOnDisplayerComponent(TSubclassOf<class UUserWidget> widgetClass, class APawn* pawn, int32 zOrder = 0);
	// Create UI and, if in VR, adds it to the VR UI Displayer Component.
	// "Diff" So this uses different UserWidgets if in VR or Flat
	UFUNCTION(BlueprintCallable, Category = "VR UI", meta = (DeterminesOutputType = "flatWidgetClass", AdvancedDisplay = "zOrder", DisplayName = "Create UI Diff (On a pawn's VR UI Displayer Component)"))
	class UUserWidget* createUIOnDisplayerComponent_Diff(UPARAM(DisplayName = "Flat Widget Class")TSubclassOf<class UUserWidget> flatWidgetClass, UPARAM(DisplayName = "VR Widget Class")TSubclassOf<class UUserWidget> VRWidgetClass, class APawn* pawn, int32 zOrder = 0);



	// Templated versions
	template<class T>
	T* createUIOnDisplayerInLevel(TSubclassOf<class UUserWidget> widgetClass, int32 zOrder = 0)
	{
		return Cast<T>(createUIOnDisplayerInLevel(widgetClass, zOrder));
	};
	template<class T>
	T* createUIOnDisplayerInLevel_Diff(TSubclassOf<class UUserWidget> flatWidgetClass, TSubclassOf<class UUserWidget> VRWidgetClass, int32 zOrder = 0)
	{
		return Cast<T>(createUIOnDisplayerInLevel_Diff(flatWidgetClass, VRWidgetClass, zOrder));
	};

	template<class T>
	T* createUIOnSpecificDisplayer(TSubclassOf<class UUserWidget> widgetClass, class AMulti_Displayer* displayer, int32 zOrder = 0)
	{
		return Cast<T>(createUIOnSpecificDisplayer(widgetClass, displayer, zOrder));
	};
	template<class T>
	T* createUIOnSpecificDisplayer_Diff(TSubclassOf<class UUserWidget> flatWidgetClass, TSubclassOf<class UUserWidget> VRWidgetClass, class AMulti_Displayer* displayer, int32 zOrder = 0)
	{
		return Cast<T>(createUIOnSpecificDisplayer_Diff(flatWidgetClass, VRWidgetClass, displayer, zOrder));
	};


	template<class T>
	T* createUIOnDisplayerComponent(TSubclassOf<class UUserWidget> widgetClass, class APawn* pawn, int32 zOrder = 0)
	{
		return Cast<T>(createUIOnDisplayerComponent(widgetClass, pawn, zOrder));
	}
	template<class T>
	T* createUIOnDisplayerComponent_Diff(TSubclassOf<class UUserWidget> flatWidgetClass, TSubclassOf<class UUserWidget> VRWidgetClass, class APawn* pawn, int32 zOrder = 0)
	{
		return Cast<T>(createUIOnDisplayerComponent_Diff(flatWidgetClass, VRWidgetClass, pawn, zOrder));
	};

protected:
	void ensureInit();

	// This is intended for use by developers only. This bool is ignored in shipping builds.
	// Setting this bool true will always use VR UI, even when not using a HMD
	UPROPERTY(EditDefaultsOnly, Category = "VR UI", DisplayName = "Force VR UI Only")
	bool bForceVRMode = false;

	UPROPERTY(BlueprintReadOnly, Category = "VR UI")
	bool bInVR = false;			// True if in VR mode
	bool bInitialized = false;
};
