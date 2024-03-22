// Fill out your copyright notice in the Description page of Project Settings.


#include "CableManager.h"

#include "Cable.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACableManager::ACableManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACableManager::BeginPlay()
{
	Super::BeginPlay();

    for (TActorIterator<ACable> It( GetWorld() , ACable::StaticClass() ); It; ++It)
    {
        ACable* Actor = *It;
        Cables.Add( Actor );
    }
	
}

void ACableManager::CheckAllMaterialsApplied()
{
    for (auto cable : Cables)
    {
	   if (false == cable->bApplyMaterial)
	   {
			// 실패
		   return;
	   }
    }
    UE_LOG( LogTemp , Warning , TEXT( "CheckAllMaterialsApplied" ) );

    // 모든 케이블의 색이 바뀜
}

