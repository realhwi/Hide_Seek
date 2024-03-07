// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOver.generated.h"

/**
 * 
 */
UCLASS()
class HIDE_SEEK_API UGameOver : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	//UPROPERTY( EditAnywhere , meta = (BindWidget) )
	//class UButton* ButtonRestart;

	//UPROPERTY( EditAnywhere , meta = (BindWidget) )
	//class UButton* ButtonQuit;

	//UPROPERTY( meta = (BindWidgetAnim) , Transient )
	//class UWidgetAnimation* StartAnim;

	UFUNCTION()
	void OnMyRestart();

	UFUNCTION()
	void OnMyQuit();
	
};
