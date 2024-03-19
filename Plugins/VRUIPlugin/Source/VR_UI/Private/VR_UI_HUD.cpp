// All Rights Reserved - 2023 - ReEngage Developments Ltd

#include "VR_UI_HUD.h"
#include "Multi_Displayer.h"
#include "Multi_DisplayerComponent.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

UUserWidget* AVR_UI_HUD::createUIOnDisplayerComponent(TSubclassOf<class UUserWidget> widgetClass, APawn* pawn, int32 zOrder)
{
	// Ensure init
	ensureInit();

	AMulti_Displayer* displayer = 0;

	// If in VR mode, get the displayer
	if (bInVR)
	{
		// Check pawn
		checkf(pawn->IsValidLowLevel(), TEXT("ERROR: Pawn not valid (Create UI on Displayer Component)"));
		if (!pawn)
			return 0;

		// Ensure pawn has begun play
		if (!pawn->HasActorBegunPlay())
			pawn->DispatchBeginPlay();

		// Attempt to get displayer component
		auto displayerComponent = pawn->GetComponentByClass<UMulti_DisplayerComponent>();
		checkf(displayerComponent->IsValidLowLevel(), TEXT("ERROR: Pawn did not have a Multi Displayer Component!"));
		if (!displayerComponent->IsValidLowLevel())
			return 0;

		// Get displayer
		displayer = displayerComponent->getDisplayer();

		// Check there is one
		checkf(displayer->IsValidLowLevel(), TEXT("ERROR: Failed to get displayer from displayer component"));
	}

	return createUIOnSpecificDisplayer(widgetClass, displayer, zOrder);
}

UUserWidget* AVR_UI_HUD::createUIOnDisplayerComponent_Diff(TSubclassOf<class UUserWidget> flatWidgetClass, TSubclassOf<class UUserWidget> VRWidgetClass, APawn* pawn, int32 zOrder)
{
	// Ensure init
	ensureInit();

	if (bInVR)
		return createUIOnDisplayerComponent(VRWidgetClass, pawn, zOrder);

	return createUIOnDisplayerComponent(flatWidgetClass, pawn, zOrder);
}

UUserWidget* AVR_UI_HUD::createUIOnDisplayerInLevel(TSubclassOf<class UUserWidget> widgetClass, int32 zOrder)
{
	// Ensure init
	ensureInit();

	// Default to null
	AMulti_Displayer* displayer = 0;

	// If in VR mode, get the displayer
	if (bInVR)
	{
		// Attempt to find displayer
		displayer = Cast<AMulti_Displayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AMulti_Displayer::StaticClass()));

		// Check there is one
		checkf(displayer->IsValidLowLevel(), TEXT("ERROR: There was not a displayer (VR_UI_Displayer) in the level when attempting to create UI in VR mode! You must place one in the level or spawn one in before calling createUIOnDisplayerInLevel."));
	}

	return createUIOnSpecificDisplayer(widgetClass, displayer, zOrder);
}

UUserWidget* AVR_UI_HUD::createUIOnDisplayerInLevel_Diff(TSubclassOf<class UUserWidget> flatWidgetClass, TSubclassOf<class UUserWidget> VRWidgetClass, int32 zOrder)
{
	// Ensure init
	ensureInit();

	if (bInVR)
		return createUIOnDisplayerInLevel(VRWidgetClass, zOrder);
	
	return createUIOnDisplayerInLevel(flatWidgetClass, zOrder);
}

UUserWidget* AVR_UI_HUD::createUIOnSpecificDisplayer(TSubclassOf<class UUserWidget> widgetClass, AMulti_Displayer* displayer, int32 zOrder)
{
	checkf(widgetClass->IsValidLowLevel(), TEXT("ERROR: Widget Class was not specified"));
	if (!widgetClass->IsValidLowLevel())
		return 0;

	// Ensure init
	ensureInit();

	// Output 
	UUserWidget* widget = 0;

	// If in VR mode
	if (bInVR)
	{
		// Ensure
		checkf(displayer, TEXT("Multi Displayer was not specified."));
		if (!displayer)
			return 0;

		widget = displayer->addUI(widgetClass, zOrder);
		check(widget);
	}
	// Else non-VR mode
	else
	{
		// Manually create the userwidget and add it to viewport
		widget = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(), 0), widgetClass);

		check(widget);
		if (!widget)
			return 0;

		widget->AddToViewport(zOrder);
	}

	return widget;
}

UUserWidget* AVR_UI_HUD::createUIOnSpecificDisplayer_Diff(TSubclassOf<class UUserWidget> flatWidgetClass, TSubclassOf<class UUserWidget> VRWidgetClass, AMulti_Displayer* displayer, int32 zOrder)
{
	// Ensure init
	ensureInit();

	if (bInVR)
		return createUIOnSpecificDisplayer(VRWidgetClass, displayer, zOrder);

	return createUIOnSpecificDisplayer(flatWidgetClass, displayer, zOrder);
}

void AVR_UI_HUD::ensureInit()
{
	if (bInitialized)
		return;
	bInitialized = true;

	// Check if in VR
	bInVR = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();

	// If not in package build
#if WITH_EDITOR

	// If user wants to force VR mode
	if (bForceVRMode)
		// Force VR
		bInVR = true;

#endif
}
