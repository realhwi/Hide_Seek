// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayerController.h"

#include "HeadMountedDisplayFunctionLibrary.h"

void AVRPlayerController::BeginPlay()
{
	Super::BeginPlay();






	if (GEngine && GEngine->StereoRenderingDevice)
	{
		GEngine->StereoRenderingDevice->EnableStereo(true);
		UE_LOG( LogTemp , Warning , TEXT( "EnableStereo" ) )
	}
}

void AVRPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	//if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected())
	//{
	//	UE_LOG( LogTemp , Warning , TEXT( "IsHeadMountedDisplayConnected" ) )
	//}
	//else
	//{
	//	UE_LOG( LogTemp , Warning , TEXT( "!IsHeadMountedDisplayConnected" ) )
	//}
	//if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	//{
	//	UE_LOG( LogTemp , Warning , TEXT( "IsHeadMountedDisplayEnabled" ) )
	//}
	//else
	//{
	//	UE_LOG( LogTemp , Warning , TEXT( "!IsHeadMountedDisplayEnabled" ) )
	//}
	//if (UHeadMountedDisplayFunctionLibrary::GetHMDWornState() == EHMDWornState::Worn)
	//{
	//	UE_LOG( LogTemp , Warning , TEXT( "Worn" ) )

	//}
	//else
	//{
	//	UE_LOG( LogTemp , Warning , TEXT( "!Worn" ) )
	//}
}
