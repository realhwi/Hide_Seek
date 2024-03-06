// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"


void UPlayerUI::AddLife()
{
	// 생명 UI를 만들고 
	auto LifeUI = CreateWidget(this,LifeUIFactory);
	// grid의 자식으로 붙인다. 
	grid_Life->AddChildToUniformGrid(LifeUI,0,grid_Life->GetChildrenCount());

	// 플레이어가 생명칩을 얻었을 때 실행 예정 
}

	// index는 0부터 시작 
void UPlayerUI::RemoveLife(int32 index)
{
	// grid의 index위치의 자식을 제거한다. 
	grid_Life->RemoveChildAt(index);
	// 플레이어가 술래랑 오버랩 되었을 때 실행 예정 
}
