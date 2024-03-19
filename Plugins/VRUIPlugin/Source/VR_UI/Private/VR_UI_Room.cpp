// All Rights Reserved - 2023 - ReEngage Developments Ltd

#include "VR_UI_Room.h"
#include "Multi_DisplayerComponent.h"

#include "Components/ChildActorComponent.h"
#include "GameFramework/PlayerStart.h"

AVR_UI_Room::AVR_UI_Room()
{
 	// Ticking disabled
	PrimaryActorTick.bCanEverTick = false;

	// Repliation disabled
	bReplicates = false;

	// Player start
	playerStartChildComponent = CreateDefaultSubobject<UChildActorComponent>("Player Start Child Actor Component");	// Spawn child actor
	SetRootComponent(playerStartChildComponent);									// Set as root component
	playerStartChildComponent->SetMobility(EComponentMobility::Static);				// Set static (for player start actor)

	// Multi Displayer Component
	displayerComponent = CreateDefaultSubobject<UMulti_DisplayerComponent>("VR UI Displayer");	// Spawn component
	displayerComponent->SetupAttachment(RootComponent);	// Attach to root

}

void AVR_UI_Room::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Child actors cannot have their class set in constructors else it prevents builds from running (at least in UE5.3)
	playerStartChildComponent->SetChildActorClass(APlayerStart::StaticClass());		// Set child actor class as player start
}

AMulti_Displayer* AVR_UI_Room::getDisplayer()
{
	check(displayerComponent);
	if (!displayerComponent)
		return 0;

	return displayerComponent->getDisplayer();
}

AActor* AVR_UI_Room::getPlayerStart()
{
	check(playerStartChildComponent->IsValidLowLevel());
	if (!playerStartChildComponent->IsValidLowLevel())
		return 0;

	// Ensure actor is created
	if (!playerStartChildComponent->GetChildActor())
		playerStartChildComponent->CreateChildActor();

	// Check it is indeed created
	check(playerStartChildComponent->GetChildActor()->IsValidLowLevel());
	if (!playerStartChildComponent->GetChildActor()->IsValidLowLevel())
		return 0;

	// Return
	return playerStartChildComponent->GetChildActor();
}

void AVR_UI_Room::BeginPlay()
{
	Super::BeginPlay();
	
}


