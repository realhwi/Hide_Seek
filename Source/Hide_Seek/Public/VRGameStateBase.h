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

	AVRGameStateBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxPlayTime = 300;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Replicated )
	float countPlayTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AVREnemyPlayer* EnemyPlayer;

	void FindEnemyPlayer();

	bool bFoundEnemy = false;

	bool bGameReady = false;

	bool bTimerReset = false;

	float seconds = 0.f;

	void CountTime();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isTimeOut = false;

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
};
