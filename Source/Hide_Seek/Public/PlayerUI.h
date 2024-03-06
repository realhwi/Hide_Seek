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
	UPROPERTY(EditDefaultsOnly,meta =(BindWidget))
	class UUniformGridPanel* grid_Life;

	UPROPERTY( EditDefaultsOnly)
	TSubclassOf<class UUserWidget>LifeUIFactory;

	// Player UI 작업 
	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite )
	TSubclassOf<class UPlayerUI> playerUIFactory;

	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite )
	class UPlayerUI* playerUI;

	// 생명칩 추가 
	void AddLife();
	// 생명칩 삭제 
	void RemoveLife( int32 index );

};
