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

	// Initialize Start Static Mesh
	StartStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "StartStaticMesh" ) );
	StartStaticMesh->SetupAttachment( RootComponent );

	// 케이블 컴포넌트 및 기타 컴포넌트 초기화
	CableComponent = CreateDefaultSubobject<UCableComponent>( TEXT( "CableComponent" ) );
	CableComponent->SetupAttachment( StartStaticMesh );
	CableComponent->CableWidth = 10.0f; // 케이블의 두께를 10으로 설정

	// Initialize MoveMesh
	MoveMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "MoveMesh" ) );
	MoveMesh->SetupAttachment( RootComponent );

	// Initialize End Sphere Collision
	EndSphereCollision = CreateDefaultSubobject<USphereComponent>( TEXT( "EndSphereCollision" ) );
	EndSphereCollision->SetupAttachment( CableComponent );
	EndSphereCollision->InitSphereRadius( 15.0f );
	EndSphereCollision->SetCollisionProfileName( TEXT( "OverlapAllDynamic" ) );

	// Initialize New End Static Mesh
	NewEndStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "NewEndStaticMesh" ) );
	NewEndStaticMesh->SetupAttachment( RootComponent );

	// Initialize New End Sphere Collision
	NewEndSphereCollision = CreateDefaultSubobject<USphereComponent>( TEXT( "NewEndSphereCollision" ) );
	NewEndSphereCollision->SetupAttachment( NewEndStaticMesh );
	NewEndSphereCollision->InitSphereRadius( 15.0f );
	NewEndSphereCollision->SetCollisionProfileName( TEXT( "OverlapAllDynamic" ) );

	// EndSphereCollision->OnComponentBeginOverlap.AddDynamic( this , &ACable::OnCableEndOverlap );
}

// Called when the game starts or when spawned
void ACable::BeginPlay()
{
	Super::BeginPlay();

	// 케이블 컴포넌트를 StartStaticMesh에 붙이기
	CableComponent->SetupAttachment( StartStaticMesh );
	
	// 케이블의 끝을 스피어 콜리전에 연결
	//CableComponent->EndLocation = EndSphereCollision->GetComponentLocation();
	CableComponent->SetAttachEndToComponent( MoveMesh , NAME_None );


	// 로그를 통해 케이블 컴포넌트와 스피어 콜리전 설정 확인
	UE_LOG( LogTemp , Warning , TEXT( "CableComponent is attached to StartStaticMesh." ) );
}

// Called every frame
void ACable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACable::HandleCableGrabbed(UPrimitiveComponent* RightController )
{

	// RightController의 유효성 검사
	if (!RightController)
	{
		UE_LOG( LogTemp , Error , TEXT( "RightController is nullptr." ) );
		return;
	}

	MoveMesh->AttachToComponent( RightController , FAttachmentTransformRules::SnapToTargetIncludingScale , FName( TEXT( "Hand_rPoint" ) ) );
	UE_LOG( LogTemp , Warning , TEXT( "Cable MoveMesh attached to RightController." ) );

	//MoveMesh->SetRelativeLocation( FVector( 0 , 0 , 0 ) );
	//MoveMesh->SetRelativeRotation( FRotator( 0 , 0 , 0 ) );
}

void ACable::HandleCableReleased()
{
	// CableComponent가 유효한지 확인
	if (!CableComponent)
	{
		UE_LOG( LogTemp , Error , TEXT( "CableComponent is nullptr." ) );
		return;
	}

	// NewEndStaticMesh가 유효한지 확인
	if (!NewEndStaticMesh)
	{
		UE_LOG( LogTemp , Error , TEXT( "NewEndStaticMesh is nullptr." ) );
		return;
	}

	// MoveMesh가 있는지 확인
	if (!MoveMesh)
	{
		UE_LOG( LogTemp , Error , TEXT( "MoveMesh is nullptr." ) );
		return;
	}

	// MoveMesh를 새로운 끝 지점에 연결
	CableComponent->SetAttachEndToComponent( NewEndStaticMesh , NAME_None );
	UE_LOG( LogTemp , Warning , TEXT( "Cable released from RightController and end location updated to NewEndStaticMesh." ) );

	// MoveMesh->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform );
	// CableComponent->EndLocation = NewEndSphereCollision->GetComponentLocation();
}




