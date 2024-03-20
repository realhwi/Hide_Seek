// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TESTSPawnGMB.generated.h"

/**
 * 
 */
UCLASS()
class HIDE_SEEK_API ATESTSPawnGMB : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATESTSPawnGMB();

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	TSubclassOf<class AHidePlayer> firstPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AVREnemyPlayer> secondPawn;

	//UPROPERTY( EditAnywhere , BlueprintReadWrite )
	//TSubclassOf<class ACharacter> secondPawn;

protected:

	UPROPERTY(Replicated)
	TSubclassOf<class ACharacter> currentPawnToAssign;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
