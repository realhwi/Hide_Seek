// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CableManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FMaterialsAppliedDelegate );

UCLASS()
class HIDE_SEEK_API ACableManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACableManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	TArray<class ACable*> CompletedCables;

	void CheckAllMaterialsApplied();

	UPROPERTY( BlueprintAssignable )
	FMaterialsAppliedDelegate  OnMaterialsApplied;
};
