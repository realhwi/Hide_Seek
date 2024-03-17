// Fill out your copyright notice in the Description page of Project Settings.


#include "Cable.h"
#include "CableComponent.h"
#include "HidePlayer.h"
#include "Components/SphereComponent.h"

// Sets default values
ACable::ACable()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>( TEXT( "RootComponent" ) );

	// 각 케이블 설정에 고유한 이름 제공
	SetupCableComponent( RedCable , "Red" );
	SetupCableComponent( BlueCable , "Blue" );
	SetupCableComponent( GreenCable , "Green" );

	// Initialize Sphere Collisions
	EndSphereCollision = CreateDefaultSubobject<USphereComponent>( TEXT( "EndSphereCollision" ) );
	EndSphereCollision->InitSphereRadius( 15.0f );
	EndSphereCollision->SetCollisionProfileName( TEXT( "OverlapAllDynamic" ) );

	NewEndSphereCollision = CreateDefaultSubobject<USphereComponent>( TEXT( "NewEndSphereCollision" ) );
	NewEndSphereCollision->InitSphereRadius( 15.0f );
	NewEndSphereCollision->SetCollisionProfileName( TEXT( "OverlapAllDynamic" ) );
}

void ACable::SetupCableComponent( FCableSetup& CableSetup , const FString& BaseName )
{
	// 고유한 이름 생성
	FName StartCompName = FName( *(BaseName + TEXT( "StartComp" )) );
	FName CableCompName = FName( *(BaseName + TEXT( "CableComp" )) );
	FName MoveCompName = FName( *(BaseName + TEXT( "MoveComp" )) );
	FName EndCompName = FName( *(BaseName + TEXT( "EndComp" )) );

	// StartMesh 초기화 및 설정
	CableSetup.StartComp = CreateDefaultSubobject<UStaticMeshComponent>( StartCompName );
	CableSetup.StartComp->SetupAttachment( RootComponent );

	// CableComponent 초기화 및 설정
	CableSetup.CableComp = CreateDefaultSubobject<UCableComponent>( CableCompName );
	CableSetup.CableComp->SetupAttachment( CableSetup.StartComp );
	CableSetup.CableComp->CableWidth = 10.0f;

	// MoveMesh 초기화 및 설정
	CableSetup.MoveComp = CreateDefaultSubobject<UStaticMeshComponent>( MoveCompName );
	CableSetup.MoveComp->SetupAttachment( RootComponent );

	// EndMesh 초기화 및 설정
	CableSetup.EndComp = CreateDefaultSubobject<UStaticMeshComponent>( EndCompName );
	CableSetup.EndComp->SetupAttachment( RootComponent );
}

void ACable::HandleCableGrabbed(UPrimitiveComponent* RightController)
{
	// RightController의 유효성 검사
	if (!RightController)
	{
		UE_LOG( LogTemp , Error , TEXT( "RightController is nullptr." ) );
		return;
	}

	// 모든 MoveMesh를 RightController에 부착
	for (FCableSetup& CableSetup : CableSetups)
	{
		if (CableSetup.MoveComp)
		{
			CableSetup.MoveComp->AttachToComponent( RightController , FAttachmentTransformRules::SnapToTargetNotIncludingScale );
			UE_LOG( LogTemp , Warning , TEXT( "Cable MoveMesh attached to RightController." ) );
		}
	}
}

void ACable::HandleCableReleased()
{
	for (FCableSetup& CableSetup : CableSetups)
	{
		// MoveMesh와 RightController의 연결을 해제합니다.
		if (CableSetup.MoveComp)
		{
			CableSetup.MoveComp->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform );
		}

		// CableComponent의 한쪽 끝을 새로운 정적 메시에 연결합니다.
		if (CableSetup.CableComp && CableSetup.EndComp)
		{
			CableSetup.CableComp->SetAttachEndToComponent( CableSetup.EndComp , NAME_None );
			UE_LOG( LogTemp , Warning , TEXT( "Cable released from RightController and end location updated to %s." ) , *CableSetup.EndComp->GetName() );
		}

		// 새로운 끝 지점에 적절한 재질을 적용합니다.
		if (CableSetup.EndComp && CableSetup.MaterialComp)
		{
			CableSetup.EndComp->SetMaterial( 0 , CableSetup.MaterialComp );
		}
	}
}

// Called when the game starts or when spawned
void ACable::BeginPlay()
{
	Super::BeginPlay();

	// CableSetups 배열을 반복하여 각 케이블을 설정합니다.
	//for (auto& CableSetup : CableSetups)
	//{
	//	// 스태틱 메시 컴포넌트와 케이블 컴포넌트를 생성합니다.
	//	CableSetup.StartComp = NewObject<UStaticMeshComponent>( this , UStaticMeshComponent::StaticClass() );
	//	CableSetup.CableComp = NewObject<UCableComponent>( this , UCableComponent::StaticClass() );
	//	CableSetup.MoveComp = NewObject<UStaticMeshComponent>( this , UStaticMeshComponent::StaticClass() );
	//	CableSetup.EndComp = NewObject<UStaticMeshComponent>( this , UStaticMeshComponent::StaticClass() );

	//	// 각 메시 컴포넌트를 루트 컴포넌트에 부착합니다.
	//	CableSetup.StartComp->SetupAttachment( RootComponent );
	//	CableSetup.MoveComp->SetupAttachment( RootComponent );
	//	CableSetup.EndComp->SetupAttachment( RootComponent );

	//	// 케이블 컴포넌트를 시작 메시에 부착하고 끝점을 이동 메시에 설정합니다.
	//	CableSetup.CableComp->SetupAttachment( CableSetup.StartComp );
	//	CableSetup.CableComp->SetAttachEndToComponent( CableSetup.MoveComp , NAME_None );

	//	// 재질을 설정합니다.
	//	if (CableSetup.MaterialComp)
	//	{
	//		CableSetup.StartComp->SetMaterial( 0 , CableSetup.MaterialComp );
	//		CableSetup.EndComp->SetMaterial( 0 , CableSetup.MaterialComp );
	//	}

	//	// 컴포넌트를 활성화하고 게임 월드에 등록합니다.
	//	CableSetup.StartComp->RegisterComponent();
	//	CableSetup.CableComp->RegisterComponent();
	//	CableSetup.MoveComp->RegisterComponent();
	//	CableSetup.EndComp->RegisterComponent();
	//}
}

//void ACable::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}
