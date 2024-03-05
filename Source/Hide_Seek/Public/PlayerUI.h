// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class HIDE_SEEK_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,meta =(BindWidget))
	class UUniformGridPanel* Life;

	UPROPERTY( EditDefaultsOnly)
	TSubclassOf<class UUserWidget>LifeUIFactory;

};
