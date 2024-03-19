// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delegates/Delegate.h"
#include "Cable.generated.h"

enum class ConnectionStage : uint8
{
	Init ,
	Mid ,
	Complete
};
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

	// 케이블 컴포넌트
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Components" )
	class UCableComponent* CableComponent;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Components" )
	class UCableComponent* CableComp1;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Components" )
	class UCableComponent* CableComp2;

	// Static Mesh for Start
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable" )
	class UStaticMeshComponent* StartStaticMesh;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable" )
	class UStaticMeshComponent* StartMesh1;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable" )
	class UStaticMeshComponent* StartMesh2;

	// Static Mesh for Start
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable" )
	class UStaticMeshComponent* MoveMesh;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable" )
	class UStaticMeshComponent* MoveMesh1;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable" )
	class UStaticMeshComponent* MoveMesh2;

	// Static Mesh for New End
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable" )
	class UStaticMeshComponent* NewEndStaticMesh;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable" )
	class UStaticMeshComponent* NewEndMesh1;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable" )
	class UStaticMeshComponent* NewEndMesh2;

	UPROPERTY( VisibleAnywhere , BlueprintReadOnly , Category = "Cable" )
	UPrimitiveComponent* CurrentlyGrabbedComp = nullptr;

	UPROPERTY( EditAnywhere , Category = "Materials" )
	UMaterialInterface* Material1;

	UPROPERTY( EditAnywhere , Category = "Materials" )
	UMaterialInterface* Material2;

	UPROPERTY( EditAnywhere , Category = "Materials" )
	UMaterialInterface* Material3;

	UPROPERTY( EditAnywhere )
	class UParticleSystem* VFX;

	UFUNCTION(BlueprintCallable)
	void HandleCableGrabbed( UPrimitiveComponent* RightController , UPrimitiveComponent* ComponentToAttach );

	UFUNCTION( BlueprintCallable )
	void HandleCableReleased( UPrimitiveComponent* NewEndComponent );

	void CheckAndApplyMaterial();
	void ResetConnectionStates();
	void ApplyMaterials();
	void ResetToInitialPositions();

	ConnectionStage GetCurrentStage() const { return CurrentStage; }

	void UpdateStage();
	void ExecuteVFX();

	
	//static TMap<int32 , bool> MaterialsAppliedStatus;
	//int32 ExpectedNumMaterialsApplied;

	//UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Cable" )
	//int32 Index; // 각 인스턴스에 고유 식별자로 사용될 변수

private:
	// 현재 연결 상태를 추적하는 변수
	ConnectionStage CurrentStage;

	bool bIsCableComponentConnected = false;
	bool bIsCableComp1Connected = false;
	bool bIsCableComp2Connected = false;

	FVector InitialMoveMeshLocation;
	FVector InitialMoveMesh1Location;
	FVector InitialMoveMesh2Location;
	FRotator InitialMoveMeshRotation;
	FRotator InitialMoveMesh1Rotation;
	FRotator InitialMoveMesh2Rotation;

	int32 ConnectionCompletedCount = 0; // 연결 완료 횟수 추적
	const int32 TotalCableComponents = 3; // 전체 케이블 컴포넌트 수

};
