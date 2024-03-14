// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayerController.h"

void AVRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine && GEngine->StereoRenderingDevice)
	{
		GEngine->StereoRenderingDevice->EnableStereo(true);
	}
}
