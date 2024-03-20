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

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnLifeDepletedDelegate );
	UPROPERTY( BlueprintAssignable , Category = "Events" )
	FOnLifeDepletedDelegate OnLifeDepleted;

	UPROPERTY(EditDefaultsOnly,meta =(BindWidget))
	class UUniformGridPanel* grid_Life;

	UPROPERTY( EditDefaultsOnly)
	TSubclassOf<class UUserWidget>LifeUIFactory;

	// 생명칩 추가
	UFUNCTION(BlueprintCallable)
	void AddLife();
	// 생명칩 삭제 
	void RemoveLife( int32 index = -1 );
};
