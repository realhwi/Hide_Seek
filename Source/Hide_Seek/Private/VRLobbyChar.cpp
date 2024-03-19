// Fill out your copyright notice in the Description page of Project Settings.


#include "VRLobbyChar.h"

// Sets default values
AVRLobbyChar::AVRLobbyChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVRLobbyChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVRLobbyChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRLobbyChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

