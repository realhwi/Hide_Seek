// All Rights Reserved - 2023 - ReEngage Developments Ltd

#include "VR_UI_Displayer_Mobile.h"
#include "Multi_WidgetComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h" 
#include "GameFramework/Pawn.h"

void AMulti_Displayer_Mobile::BeginPlay()
{
	Super::BeginPlay();

	// Client only (ideally, servers shouldn't even have this actor but they might if it's a level actor)
	if (GetNetMode() == ENetMode::NM_Client || GetNetMode() == ENetMode::NM_Standalone || GetNetMode() == ENetMode::NM_ListenServer)
	{
		localPlayerController = GetWorld()->GetFirstPlayerController();
		check(localPlayerController);
	}
}

AMulti_Displayer_Mobile::AMulti_Displayer_Mobile()
{

}

void AMulti_Displayer_Mobile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If not hidden and wants to follow player
	if (!IsHidden() && bFollowPlayer)
	{
		// If local player controller is set (from BeginPlay)
		if (localPlayerController)
		{
			// If there is a pawn
			if (const auto* pawn = localPlayerController->GetPawnOrSpectator())
			{
				// Get and set location
				const auto floorLocation = pawn->GetActorLocation() + (pawn->GetActorForwardVector() * distanceFromPlayer);
				const auto finalLocation = floorLocation + FVector(0, 0, (widgetComponentTemplate->GetCurrentDrawSize().Y / 2 * widgetComponentTemplate->GetComponentScale().Z));
				SetActorLocation(finalLocation);

				// Rotate to look at player pawn
				const auto rotator = UKismetMathLibrary::FindLookAtRotation(floorLocation, pawn->GetActorLocation());
				SetActorRotation(rotator);
			}
		}
	}
}

void AMulti_Displayer_Mobile::setVisible(bool bVisible)
{
	// Set bool
	bDisplayerVisible = bVisible;

	// Set hidden or not
	SetActorHiddenInGame(!bDisplayerVisible);
}

void AMulti_Displayer_Mobile::toggleVisibility()
{
	setVisible(!bDisplayerVisible);
}

void AMulti_Displayer_Mobile::setFollowPlayer(bool bFollowPlayer_)
{
	// Set bool
	bFollowPlayer = bFollowPlayer_;
}