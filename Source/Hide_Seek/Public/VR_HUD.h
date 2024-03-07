// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VR_HUD.generated.h"

/**
 * 
 */
UCLASS()
class HIDE_SEEK_API AVR_HUD : public AHUD
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
