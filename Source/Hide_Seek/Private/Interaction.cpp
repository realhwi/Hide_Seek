// Fill out your copyright notice in the Description page of Project Settings.

#include "Interaction.h"
#include "HidePlayer.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AInteraction::AInteraction()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInteraction::BeginPlay()
{
	Super::BeginPlay();

	 AHidePlayer* PlayerCharacter = Cast<AHidePlayer>( UGameplayStatics::GetPlayerCharacter( GetWorld() , 0 ) );
	 if (PlayerCharacter)
	 {
		 OnTriggerInteract( PlayerCharacter );
		 OnGrabInteract( PlayerCharacter );
	 }
}

// Called every frame
void AInteraction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* AHidePlayer* PlayerCharacter = Cast<AHidePlayer>( UGameplayStatics::GetPlayerCharacter( GetWorld() , 0 ) );
	 if (PlayerCharacter)
	 {
	 	OnInteract( PlayerCharacter );
	 }*/
}



