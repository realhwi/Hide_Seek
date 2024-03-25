// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HidePlayer.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"


UCLASS()
class HIDE_SEEK_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnLifeDepletedDelegate );

	UPROPERTY( BlueprintAssignable , Category = "Events" )
	FOnLifeDepletedDelegate OnLifeDepleted;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UWidgetSwitcher* WidgetSwitcherInventory;

	UPROPERTY(EditDefaultsOnly,meta =(BindWidget))
	class UUniformGridPanel* grid_Life;

	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<class UUserWidget>LifeUIFactory;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UButton* btn_hidden;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UImage* hidden;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UButton* btn_speed;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UImage* speed;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UButton* btn_back;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UButton* btn_power_red;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UImage* red;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UButton* btn_power_blue;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UImage* blue;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UButton* btn_power_green;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UImage* green;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UButton* btn_exit_red;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UButton* btn_exit_blue;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UButton* btn_exit_green;


	// 생명칩 추가, 삭제 
	UFUNCTION(BlueprintCallable)
	void AddLife();
	void RemoveLife( int32 index = -1 );

	// 판넬 인덱스 
	void SwitchPanel( int32 index );

	// 인벤토리 나가기 
	UFUNCTION()
	void ExitInventory();

	// 인벤토리 뒤로가기
	UFUNCTION()
	void Back_red();
	UFUNCTION()
	void Back_blue();
	UFUNCTION()
	void Back_green();

	// 설계도 획득

	// 인벤토리에서 설계도 확인 
	UFUNCTION()
	void GoRedMap();
	UFUNCTION()
	void GoBlueMap();
	UFUNCTION()
	void GoGreenMap();

	// 아이템 획득
	UFUNCTION()
	void GetHiddenItem();
	UFUNCTION()
	void GetSpeedItem();


	// 아이템 사용 
};
