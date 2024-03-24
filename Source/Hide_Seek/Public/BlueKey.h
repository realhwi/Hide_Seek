// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlueKey.generated.h"

UCLASS()
class HIDE_SEEK_API ABlueKey : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlueKey();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Effects" )
	class UStaticMeshComponent* blueComp;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Sphere" )
	class USphereComponent* SphereComp;

	UFUNCTION()
	void OnOverlapBegin( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult );

};
