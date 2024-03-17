// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CableComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Cable.generated.h"


USTRUCT( Atomic,BlueprintType )
struct FCableSetup
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	UCableComponent* CableComp;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	UStaticMeshComponent* StartComp;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	UStaticMeshComponent* MoveComp;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	UStaticMeshComponent* EndComp;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	UMaterialInterface* MaterialComp;
};

UCLASS()
class HIDE_SEEK_API ACable : public AActor
{
	GENERATED_BODY()
	
public:	
	ACable();

protected:
	virtual void BeginPlay() override;

public:
	// virtual void Tick( float DeltaTime ) override;
	void SetupCableComponent( FCableSetup& CableSetup , const FString& BaseName );

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable")
	TArray<FCableSetup> CableSetups;

	UPROPERTY( EditAnywhere , BlueprintReadOnly , Category = "Cable" )
	USphereComponent* EndSphereCollision;

	UPROPERTY( EditAnywhere , BlueprintReadOnly , Category = "Cable" )
	USphereComponent* NewEndSphereCollision;

	UPROPERTY( EditAnywhere,BlueprintReadWrite,Category = "Cable", Meta = (ExposeOnSpawn = true) )
	FCableSetup RedCable;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable" )
	UStaticMeshComponent* RedStartComp;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable", Meta = (ExposeOnSpawn = true) )
	FCableSetup BlueCable;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable" )
	UStaticMeshComponent* BlueStartComp;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable", Meta = (ExposeOnSpawn = true) )
	FCableSetup GreenCable;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable" )
	UStaticMeshComponent* GreenStartComp;


	UFUNCTION( BlueprintCallable )
	void HandleCableGrabbed( UPrimitiveComponent* RightController );

	UFUNCTION( BlueprintCallable )
	void HandleCableReleased();

};
