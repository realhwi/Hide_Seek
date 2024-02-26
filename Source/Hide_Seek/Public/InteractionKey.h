// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "InteractionKey.generated.h"

UCLASS()
class HIDE_SEEK_API AInteractionKey : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractionKey();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Static Mesh 컴포넌트 추가 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* meshComp;

	// 콜리전 박스 추가 
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UBoxComponent* boxComp;

	// 상호작용이 감지되었을 때 호출하는 함수 
	UFUNCTION()
	void OnInteractionBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
