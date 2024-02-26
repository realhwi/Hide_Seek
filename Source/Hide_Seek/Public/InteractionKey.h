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

	// Static Mesh ������Ʈ �߰� 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* meshComp;

	// �ݸ��� �ڽ� �߰� 
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UBoxComponent* boxComp;

	// ��ȣ�ۿ��� �����Ǿ��� �� ȣ���ϴ� �Լ� 
	UFUNCTION()
	void OnInteractionBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
