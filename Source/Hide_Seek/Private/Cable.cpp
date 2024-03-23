// Fill out your copyright notice in the Description page of Project Settings.


#include "Cable.h"
#include "CableComponent.h"
#include "CableManager.h"
#include "HidePlayer.h"
#include "Kismet/GameplayStatics.h"


// TMap<int32 , bool> ACable::MaterialsAppliedStatus;
ACable::ACable()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>( TEXT( "RootComponent" ) );

	// Initialize Start Static Mesh
	StartStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "StartStaticMesh" ) );
	StartStaticMesh->SetupAttachment( RootComponent );
	StartMesh1 = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "StartMesh1" ) );
	StartMesh1->SetupAttachment( RootComponent );
	StartMesh2 = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "StartMesh2" ) );
	StartMesh2->SetupAttachment( RootComponent );

	// 케이블 컴포넌트 및 기타 컴포넌트 초기화
	CableComponent = CreateDefaultSubobject<UCableComponent>( TEXT( "CableComponent" ) );
	CableComponent->SetupAttachment( StartStaticMesh );
	CableComponent->CableWidth = 10.0f; // 케이블의 두께를 10으로 설정

	CableComp1 = CreateDefaultSubobject<UCableComponent>( TEXT( "CableComp1" ) );
	CableComp1->SetupAttachment( StartMesh1 );
	CableComp1->CableWidth = 10.0f; // 케이블의 두께를 10으로 설정

	CableComp2 = CreateDefaultSubobject<UCableComponent>( TEXT( "CableComp2" ) );
	CableComp2->SetupAttachment( StartMesh2 );
	CableComp2->CableWidth = 10.0f; // 케이블의 두께를 10으로 설정

	// Initialize MoveMesh
	MoveMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "MoveMesh" ) );
	MoveMesh->SetupAttachment( RootComponent );
	MoveMesh1 = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "MoveMesh1" ) );
	MoveMesh1->SetupAttachment( RootComponent );
	MoveMesh2 = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "MoveMesh2" ) );
	MoveMesh2->SetupAttachment( RootComponent );

	// Initialize New End Static Mesh
	NewEndStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "NewEndStaticMesh" ) );
	NewEndStaticMesh->SetupAttachment( RootComponent );
	NewEndMesh1 = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "NewEndMesh1" ) );
	NewEndMesh1->SetupAttachment( RootComponent );
	NewEndMesh2 = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "NewEndMesh2" ) );
	NewEndMesh2->SetupAttachment( RootComponent );

}

// Called when the game starts or when spawned
void ACable::BeginPlay()
{
	Super::BeginPlay();

	// 케이블 컴포넌트를 StartStaticMesh에 붙이기
	CableComponent->SetupAttachment( StartStaticMesh );
	CableComp1->SetupAttachment( StartMesh1 );
	CableComp2->SetupAttachment( StartMesh2 );

	// 케이블의 끝을 스피어 콜리전에 연결
	CableComponent->SetAttachEndToComponent( MoveMesh , NAME_None );
	CableComp1->SetAttachEndToComponent( MoveMesh1 , NAME_None );
	CableComp2->SetAttachEndToComponent( MoveMesh2 , NAME_None );

	// 초기 위치 및 회전값 저장
	InitialMoveMeshLocation = MoveMesh->GetComponentLocation();
	InitialMoveMeshRotation = MoveMesh->GetComponentRotation();

	InitialMoveMesh1Location = MoveMesh1->GetComponentLocation();
	InitialMoveMesh1Rotation = MoveMesh1->GetComponentRotation();

	InitialMoveMesh2Location = MoveMesh2->GetComponentLocation();
	InitialMoveMesh2Rotation = MoveMesh2->GetComponentRotation();


	//Manager = Cast<ACableManager>( UGameplayStatics::GetActorOfClass( GetWorld() , ACableManager::StaticClass() ) );
}

void ACable::HandleCableGrabbed( UPrimitiveComponent* RightController , UPrimitiveComponent* ComponentToAttach )
{
	// RightController의 유효성 검사
	if (!RightController)
	{
		//UE_LOG( LogTemp , Error , TEXT( "RightController is nullptr." ) );
		return;
	}

	// ComponentToAttach를 RightController에 부착
	ComponentToAttach->AttachToComponent( RightController , FAttachmentTransformRules::SnapToTargetNotIncludingScale );

	// 현재 그랩한 컴포넌트로 설정
	CurrentlyGrabbedComp = ComponentToAttach;
	UE_LOG( LogTemp , Warning , TEXT( "Component attached to RightController." ) );

}

void ACable::HandleCableReleased( UPrimitiveComponent* NewEndComponent )
{
	if (!CurrentlyGrabbedComp || !NewEndComponent)
	{
		UE_LOG( LogTemp , Warning , TEXT( "Either grabbed component or NewEndComponent is null." ) );
		return;
	}

	CurrentlyGrabbedComp->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform );
	CurrentlyGrabbedComp->AttachToComponent( NewEndComponent , FAttachmentTransformRules::SnapToTargetNotIncludingScale );
	//UE_LOG( LogTemp , Warning , TEXT( "Attempted to attach %s to %s." ) , *GetNameSafe( CurrentlyGrabbedComp ) , *GetNameSafe( NewEndComponent ) );


	// 케이블 컴포넌트의 연결 상태 확인 및 업데이트
	if (CurrentlyGrabbedComp == MoveMesh && NewEndComponent == NewEndStaticMesh)
	{
		bIsCableComponentConnected = true;
		//ConnectionCompletedCount++;

	}
	else if (CurrentlyGrabbedComp == MoveMesh1 && NewEndComponent == NewEndMesh1)
	{
		bIsCableComp1Connected = true;
		//ConnectionCompletedCount++;
	}
	else if (CurrentlyGrabbedComp == MoveMesh2 && NewEndComponent == NewEndMesh2)
	{
		bIsCableComp2Connected = true;
		//ConnectionCompletedCount++;
	}
	UE_LOG( LogTemp , Warning , TEXT( "ConnectionCompletedCount.Add" ) );
	ConnectionCompletedCount++;
	//UE_LOG( LogTemp , Warning , TEXT( "All cable components successfully connected." ) );
	// 머터리얼 변경 또는 초기 위치로 복귀하는 로직 호출
	CheckAndApplyMaterial();
	
}

void ACable::CheckAndApplyMaterial()
{
	UE_LOG( LogTemp , Warning , TEXT( "Checking cable connections. ConnectionCompletedCount: %d, Expected: %d" ) , ConnectionCompletedCount , TotalCableComponents );

	// 모든 케이블 컴포넌트의 연결이 완료되었는지 확인
	if (ConnectionCompletedCount == TotalCableComponents)
	{
		if (bIsCableComponentConnected && bIsCableComp1Connected && bIsCableComp2Connected)
		{
			UE_LOG( LogTemp , Warning , TEXT( "All connections valid, applying materials." ) );
			ApplyMaterials();
		}
		else
		{
			UE_LOG( LogTemp , Warning , TEXT( "One or more connections invalid, resetting to initial positions." ) );

			// 하나라도 연결이 실패했다면 초기 위치로 복귀
			ResetToInitialPositions();
		}

		// 연결 상태 변수 및 연결 완료 횟수 초기화
		UE_LOG( LogTemp , Warning , TEXT( "Not all connections valid, resetting to initial positions." ) );

		ResetConnectionStates();
	}
}

void ACable::ApplyMaterials()
{
	// 세 컴포넌트 세트가 각각 올바르게 연결된 경우, 머터리얼 변경
	StartStaticMesh->SetMaterial( 0 , Material1 );
	CableComponent->SetMaterial( 0 , Material1 );
	NewEndStaticMesh->SetMaterial( 0 , Material1 );

	StartMesh1->SetMaterial( 0 , Material2 );
	CableComp1->SetMaterial( 0 , Material2 );
	NewEndMesh1->SetMaterial( 0 , Material2 );

	StartMesh2->SetMaterial( 0 , Material3 );
	CableComp2->SetMaterial( 0 , Material3 );
	NewEndMesh2->SetMaterial( 0 , Material3 );

	bApplyMaterial = true;
	UE_LOG( LogTemp , Warning , TEXT( "bApplyMaterial=true" ) );

	//Manager->CheckAllMaterialsApplied( );
}

void ACable::ResetToInitialPositions()
{
	// Log current positions before resetting
	/*UE_LOG( LogTemp , Warning , TEXT( "Resetting positions. Current positions: MoveMesh: %s, MoveMesh1: %s, MoveMesh2: %s" ) ,
		*MoveMesh->GetComponentLocation().ToString() ,
		*MoveMesh1->GetComponentLocation().ToString() ,
		*MoveMesh2->GetComponentLocation().ToString() );*/

	// NewEndStaticMesh, NewEndMesh1, NewEndMesh2에 붙어 있던 End 지점을 떼는 작업
	if (bIsCableComponentConnected)
	{
		CableComponent->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform );
		// CableComponent를 MoveMesh로 다시 Attach
		CableComponent->AttachToComponent( MoveMesh , FAttachmentTransformRules::SnapToTargetNotIncludingScale );
	}

	if (bIsCableComp1Connected)
	{
		CableComp1->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform );
		// CableComp1을 MoveMesh1로 다시 Attach
		CableComp1->AttachToComponent( MoveMesh1 , FAttachmentTransformRules::SnapToTargetNotIncludingScale );
	}

	if (bIsCableComp2Connected)
	{
		CableComp2->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform );
		// CableComp2를 MoveMesh2로 다시 Attach
		CableComp2->AttachToComponent( MoveMesh2 , FAttachmentTransformRules::SnapToTargetNotIncludingScale );
	}

	MoveMesh->SetWorldLocation( InitialMoveMeshLocation );
	MoveMesh1->SetWorldLocation( InitialMoveMesh1Location );
	MoveMesh2->SetWorldLocation( InitialMoveMesh2Location );

	// Log new positions after resetting
	//UE_LOG( LogTemp , Warning , TEXT( "New positions after resetting: MoveMesh: %s, MoveMesh1: %s, MoveMesh2: %s" ) ,
	//	*MoveMesh->GetComponentLocation().ToString() ,
	//	*MoveMesh1->GetComponentLocation().ToString() ,
	//	*MoveMesh2->GetComponentLocation().ToString() );
}

void ACable::ResetConnectionStates()
{
	//UE_LOG( LogTemp , Warning , TEXT( "Resetting connection states." ) );
	// 연결 상태 변수 및 연결 완료 횟수 초기화
	bIsCableComponentConnected = false;
	bIsCableComp1Connected = false;
	bIsCableComp2Connected = false;
	ConnectionCompletedCount = 0;
}
