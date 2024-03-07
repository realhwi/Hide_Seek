// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"


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

    if (ChildCountAfter  == 0)
    {
        UE_LOG( LogTemp , Warning , TEXT( "All lives depleted." ) );
        OnLifeDepleted.Broadcast();
    }
}

