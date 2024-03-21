// Fill out your copyright notice in the Description page of Project Settings.


#include "VRGameStateBase.h"

#include "Net/UnrealNetwork.h"

void AVRGameStateBase::BeginPlay()
{
	Super::BeginPlay();
}

void AVRGameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

//void AVRGameStateBase::ServerRPC_PlayTime_Implementation()
//{
//
//}

void AVRGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME( AVRGameStateBase , countPlayTime );

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
