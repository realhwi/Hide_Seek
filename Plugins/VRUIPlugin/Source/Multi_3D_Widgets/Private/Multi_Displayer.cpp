// All Rights Reserved - 2024 - ReEngage Developments Ltd

#include "Multi_Displayer.h"
#include "Multi_WidgetComponent.h"

#include "Components/WidgetComponent.h"
#include "UObject/NameTypes.h"

AMulti_Displayer::AMulti_Displayer()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;		// Does not replicate

	widgetComponentTemplate = CreateDefaultSubobject<UMulti_WidgetComponent>("Widget Template");
	check(widgetComponentTemplate);
	SetRootComponent(widgetComponentTemplate);
	widgetComponentTemplate->SetCollisionProfileName(TEXT("NoCollision"));
}

void AMulti_Displayer::BeginPlay()
{
	// Client only (ideally, servers shouldn't even have this actor but they might if it's a level actor)
	if (GetNetMode() == ENetMode::NM_Client || GetNetMode() == ENetMode::NM_Standalone || GetNetMode() == ENetMode::NM_ListenServer)
	{
		// Process the widget template
		check(widgetComponentTemplate);
		if (widgetComponentTemplate)	// Should never not be set
		{
			// Should never have a widget (you left one of these widgets with a class. 
			// It will get destroyed so it's fine but still it's a waste of computation
			UUserWidget* widget = widgetComponentTemplate->GetWidget();
			ensureMsgf(!widget, TEXT("A widget class was set on the template widget component in a Multi_Displayer. This isn't a problem but wastes a bit of performance on BeginPlay."));
			if (widget)
			{
				widget->RemoveFromRoot();
				widget->MarkAsGarbage();
				widgetComponentTemplate->SetWidget(0);
			}

			// Ensure no collision
			widgetComponentTemplate->SetCollisionProfileName(TEXT("NoCollision"));
		}
	}

	// Call super now
	Super::BeginPlay();
}

UUserWidget* AMulti_Displayer::addUI(TSubclassOf<UUserWidget> widgetClass, int32 zOrder)
{
	checkf(widgetClass, TEXT("Invalid widgetClass was sent to AMulti_Displayer::addUI()"));
	if (!widgetClass->IsValidLowLevel())
		return 0;

	// Ensure begun play
	if (!HasActorBegunPlay() && !IsActorBeginningPlay())
		DispatchBeginPlay();

	// Create widget component
	UMulti_WidgetComponent* newWidget = NewObject<UMulti_WidgetComponent>(this, UMulti_WidgetComponent::StaticClass());		// Construct widget
	if (!newWidget->IsValidLowLevel())
		return 0;

	// Setup
	newWidget->SetupAttachment(RootComponent);
	newWidget->RegisterComponent();
	if (newWidget->bWantsInitializeComponent)
		newWidget->InitializeComponent();
	newWidget->SetInitialLayerZOrder(zOrder);

	// Copy template widget
	newWidget->copyProperties(widgetComponentTemplate);

	// Setup custom channel if used
	if (bUseCustomChannel)
		newWidget->setCustomChannel(customChannel);

	// Set widget
	newWidget->SetWidgetClass(widgetClass);

	// Ensure widget spawned
	check(newWidget->GetWidget()->IsValidLowLevel());
	if (!newWidget->GetWidget()->IsValidLowLevel())
		return 0;

	// Set widget owner
	newWidget->GetWidget()->SetOwningPlayer(GetWorld()->GetFirstPlayerController());

	// Sort widget into correct zOrder struct
	addtoZOrderStruct(newWidget, zOrder);

	// Apply z order to all widgets
	applyZOrder();

	// Return
	return newWidget->GetWidget();
}

void AMulti_Displayer::addtoZOrderStruct(UMulti_WidgetComponent* widget, int32 zOrder)
{
	// Find struct
	FZOrderStruct* zOrderStruct = 0;
	for (int i = 0; i < widgets.Num(); i++)
	{
		if (widgets[i].zOrder == zOrder)
		{
			zOrderStruct = &widgets[i];
			break;
		}
	}

	// If struct wasn't found, create one
	if (!zOrderStruct)
	{
		widgets.Add(FZOrderStruct(zOrder));
		zOrderStruct = &widgets.Last();
	}

	// Add this widget to the struct
	check(zOrderStruct);
	if (zOrderStruct)
		zOrderStruct->widgets.Add(widget);
}

void AMulti_Displayer::applyZOrder()
{
	// Sort TArray by zOrder. This sets zOrder 0 first, etc.
	// You're not going to have thousands of widgets so performance is a non-issue
	widgets.Sort();

	// Tracker for current dist
	double dist = 0;
	constexpr double distIncrement = 0.2;

	// Apply dist
	for (auto& zOrderStruct : widgets)
	{
		for (auto* widget : zOrderStruct.widgets)
		{
			// Set location
			widget->SetRelativeLocation(FVector(dist, 0.0, 0.0));

			// Increment dist
			dist += distIncrement;
		}
	}
}

void AMulti_Displayer::setCustomChannel(ECollisionChannel newCustomChannel)
{
	customChannel = newCustomChannel;
	bUseCustomChannel = true;

	// Update existing widgets
	for (auto &zOrderStruct : widgets)
	{
		for (auto* widgetComponent : zOrderStruct.widgets)
		{
			if (widgetComponent)
				widgetComponent->setCustomChannel(customChannel);
		}
	}
}

void AMulti_Displayer::setDontUseCustomChannel()
{
	bUseCustomChannel = false;
	
	// Update existing widgets
	for (auto& zOrderStruct : widgets)
	{
		for (auto* widgetComponent : zOrderStruct.widgets)
		{
			if (widgetComponent)
				widgetComponent->setDontUseCustomChannel();
		}
	}
}
