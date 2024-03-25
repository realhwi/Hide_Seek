// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "VREnemyPlayer.h"
#include "Kismet/GameplayStatics.h"

const int SWITCHER_INDEX_ITEM = 0;
const int SWITCHER_INDEX_REDMAP = 1;
const int SWITCHER_INDEX_BLUEMAP = 2;
const int SWITCHER_INDEX_GREENMAP = 3;

void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();

    btn_back->OnClicked.AddDynamic( this , &UPlayerUI::ExitInventory );
    btn_power_red->OnClicked.AddDynamic( this , &UPlayerUI::GoRedMap );
    btn_power_blue->OnClicked.AddDynamic( this , &UPlayerUI::GoBlueMap );
    btn_power_green->OnClicked.AddDynamic( this , &UPlayerUI::GoGreenMap );
    btn_exit_red->OnClicked.AddDynamic( this , &UPlayerUI::Back_red );
    btn_exit_blue->OnClicked.AddDynamic( this , &UPlayerUI::Back_blue );
    btn_exit_green->OnClicked.AddDynamic( this , &UPlayerUI::Back_green );
    btn_hidden->OnClicked.AddDynamic( this , &UPlayerUI::GetHiddenItem );
    btn_speed->OnClicked.AddDynamic( this , &UPlayerUI::GetSpeedItem );

    hidden->SetVisibility( ESlateVisibility::Hidden );
    speed->SetVisibility( ESlateVisibility::Hidden );
    red->SetVisibility( ESlateVisibility::Hidden );
    blue->SetVisibility( ESlateVisibility::Hidden );
    green->SetVisibility( ESlateVisibility::Hidden );
}

void UPlayerUI::AddLife()
{
    // 생명 UI를 만들고
    auto LifeUI = CreateWidget<UUserWidget>( GetOwningPlayer() , LifeUIFactory );
    if (LifeUI != nullptr)
    {
        // grid의 자식으로 붙인다.
        grid_Life->AddChildToUniformGrid( LifeUI , 0 , grid_Life->GetChildrenCount() );
    }
}

	// index는 0부터 시작 
void UPlayerUI::RemoveLife( int32 index )
{
    int32 ChildCountBefore = grid_Life->GetChildrenCount();
    UE_LOG( LogTemp , Warning , TEXT( "Before removal, child count: %d" ) , ChildCountBefore );

    if (ChildCountBefore > 0)
    {
        if (index < 0 || index >= ChildCountBefore) // 인덱스가 제공되지 않았거나 범위를 벗어난 경우
        {
            index = ChildCountBefore - 1; // 마지막 인덱스 선택
        }
        grid_Life->RemoveChildAt( index );
    }

    int32 ChildCountAfter = grid_Life->GetChildrenCount();

    UE_LOG( LogTemp , Warning , TEXT( "After removal, child count: %d" ) , ChildCountAfter );

    if (ChildCountAfter == 0)
    {
        UE_LOG( LogTemp , Warning , TEXT( "All lives depleted." ) );
        OnLifeDepleted.Broadcast();
    }
}

void UPlayerUI::SwitchPanel(int32 index)
{
    WidgetSwitcherInventory->SetActiveWidgetIndex( index );
}


void UPlayerUI::ExitInventory()
{
    AHidePlayer* Player = Cast<AHidePlayer>( GetOwningPlayerPawn() );
    if (Player)
    {
        Player->InventoryVisibility();
    }
}

void UPlayerUI::Back_red()
{
    SwitchPanel( SWITCHER_INDEX_ITEM );
}

void UPlayerUI::Back_blue()
{

    SwitchPanel( SWITCHER_INDEX_ITEM );
}

void UPlayerUI::Back_green()
{
    SwitchPanel( SWITCHER_INDEX_ITEM );
}

void UPlayerUI::GoRedMap()
{
    if (red->Visibility == ESlateVisibility::Visible)
    {
        SwitchPanel( SWITCHER_INDEX_REDMAP );
    }
}

void UPlayerUI::GoBlueMap()
{
    if (blue->Visibility == ESlateVisibility::Visible)
    {
        SwitchPanel( SWITCHER_INDEX_BLUEMAP );
    }
}

void UPlayerUI::GoGreenMap()
{
    if (blue->Visibility == ESlateVisibility::Visible)
    {
        SwitchPanel( SWITCHER_INDEX_GREENMAP );
    }
}

void UPlayerUI::GetHiddenItem()
{
    if (hidden->Visibility == ESlateVisibility::Visible)
    {
        //Server World의 내가 Overlap
        AHidePlayer* Player = Cast<AHidePlayer>( GetOwningPlayerPawn() );
        if (Player && Player->HasAuthority())
        {
            Player->ServerChangeVisibility();
            hidden->SetVisibility( ESlateVisibility::Hidden );
        }
    }
}

void UPlayerUI::GetSpeedItem()
{
    if (speed->Visibility == ESlateVisibility::Visible)
    {
        if (AVREnemyPlayer* VREnemyPlayer = Cast<AVREnemyPlayer>( UGameplayStatics::GetPlayerCharacter( GetWorld() , 0 ) ))
        {
  			UE_LOG( LogTemp , Warning , TEXT( "ChangeSpeed." ) );
  			VREnemyPlayer->ChangeSpeed();
            speed->SetVisibility( ESlateVisibility::Hidden );
		}
    }
}

