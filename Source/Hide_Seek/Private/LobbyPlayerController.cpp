// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine && GEngine->StereoRenderingDevice)
	{
		GEngine->StereoRenderingDevice->EnableStereo(false);
	}
}
