// Fill out your copyright notice in the Description page of Project Settings.


#include "CableManager.h"

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
	
}

void ACableManager::CheckAllMaterialsApplied()
{
    if (CompletedCables.Num() == 3)
    {
        OnMaterialsApplied.Broadcast();
    }
}

