// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction.generated.h"


class AHidePlayer;

UCLASS()
class HIDE_SEEK_API AInteraction : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteraction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 블루프린트에서 구현할 트리거 상호작용 함수
	UFUNCTION( BlueprintImplementableEvent , Category = "Interaction" )
	void OnTriggerInteract( AHidePlayer* Player );

	// 블루프린트에서 구현할 그랩 상호작용 함수
	UFUNCTION( BlueprintImplementableEvent , Category = "Interaction" )
	void OnGrabInteract( AHidePlayer* Player );

	UFUNCTION( BlueprintImplementableEvent , Category = "Interaction" )
	void OnCableGrabbed( AHidePlayer* Player );

	UFUNCTION( BlueprintImplementableEvent , Category = "Interaction" )
	void OnUnGrabInteract( AHidePlayer* Player );
};
