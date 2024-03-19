// All Rights Reserved - 2024 - ReEngage Developments Ltd

#include "Multi_DisplayerComponent.h"
#include "Multi_Displayer.h"
#include "Multi_WidgetComponent.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/HUD.h"
#include "Components/ChildActorComponent.h"

UMulti_DisplayerComponent::UMulti_DisplayerComponent()
{
	SetIsReplicated(false);
}

void UMulti_DisplayerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Should never have a widget (you left one of these widgets with a class. 
	// It will get destroyed so it's fine but still it's a waste of computation
	UUserWidget* widget = GetWidget();
	ensureMsgf(!widget, TEXT("A widget class was set on the template widget component in a Multi_Displayer_Component. This isn't a problem but wastes a bit of performance on BeginPlay."));
	if (widget)
	{
		widget->RemoveFromRoot();
		widget->MarkAsGarbage();
		SetWidget(0);
	}

	// Ensure no collision
	SetCollisionProfileName(TEXT("NoCollision"));

	spawnMulti_Displayer();
}

void UMulti_DisplayerComponent::spawnMulti_Displayer()
{
	// Create child actor component
	auto childActorComponent = NewObject<UChildActorComponent>(this);
	check(childActorComponent->IsValidLowLevel());
	if (!childActorComponent)
		return;

	// Register component
	childActorComponent->RegisterComponent();
	childActorComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// Set child actor and spawn Multi Displayer
	childActorComponent->SetChildActorClass(AMulti_Displayer::StaticClass());
	check(childActorComponent->GetChildActor()->IsValidLowLevel());
	displayer = Cast<AMulti_Displayer>(childActorComponent->GetChildActor());
	displayer->SetActorRelativeScale3D(GetRelativeScale3D());
	//childActorComponent->CreateChildActor();

	// Setup custom collision
	if (bUseCustomChannel)
		displayer->setCustomChannel(customChannel);

	// setup Multi Displayer
	displayer->getWidgetComponentTemplate()->copyProperties(this);
}
