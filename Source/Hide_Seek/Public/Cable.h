// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cable.generated.h"

class AHidePlayer;

UCLASS()
class HIDE_SEEK_API ACable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 케이블 컴포넌트
	UPROPERTY( VisibleAnywhere , BlueprintReadOnly , Category = "Components" )
	class UCableComponent* CableComponent;

	// Static Mesh for Start
	UPROPERTY( VisibleAnywhere , BlueprintReadOnly , Category = "Cable" )
	class UStaticMeshComponent* StartStaticMesh;

	// Sphere Collision for End
	UPROPERTY( VisibleAnywhere , BlueprintReadOnly , Category = "Cable" )
	class USphereComponent* EndSphereCollision;

	// Static Mesh for New End
	UPROPERTY( VisibleAnywhere , BlueprintReadOnly , Category = "Cable" )
	class UStaticMeshComponent* NewEndStaticMesh;

	// Sphere Collision for New End
	UPROPERTY( VisibleAnywhere , BlueprintReadOnly , Category = "Cable" )
	USphereComponent* NewEndSphereCollision;

	// 케이블 끝점 드래그 여부
	UPROPERTY( VisibleAnywhere , BlueprintReadOnly , Category = "Gameplay" )
	bool bIsDraggingCableEnd = false;

	// 플레이어의 RightController 위치를 저장하는 변수
	FVector PlayerRightControllerLocation;

	UFUNCTION( BlueprintCallable )
	void SetCableEndLocation( const FVector& NewEndLocation );

	UFUNCTION()
	void OnCableEndOverlap( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult );

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	AHidePlayer* OwningPlayer;
};
