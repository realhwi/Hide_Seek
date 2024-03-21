// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "VRGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class HIDE_SEEK_API AVRGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Replicated )
	float countPlayTime = 300;

	//UFUNCTION(Server, Reliable)
	//void ServerRPC_PlayTime();

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	
};
