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

	// 케이블 컴포넌트를 StartStaticMesh에 붙입니다.
	CableComponent->SetupAttachment( StartStaticMesh );
	
	// 케이블의 끝을 스피어 콜리전에 연결
	//CableComponent->EndLocation = EndSphereCollision->GetComponentLocation();
	CableComponent->SetAttachEndToComponent( EndSphereCollision , NAME_None );


	// 로그를 통해 케이블 컴포넌트와 스피어 콜리전 설정 확인
	UE_LOG( LogTemp , Warning , TEXT( "CableComponent is attached to StartStaticMesh." ) );
	UE_LOG( LogTemp , Warning , TEXT( "CableComponent EndLocation is set to: %s" ) , *CableComponent->EndLocation.ToString() );
}

// Called every frame
void ACable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	 // 케이블이 잡혀 있는지 확인
	if (OwningPlayer && OwningPlayer->IsGrab())
	{
		// 잡고 있는 오브젝트가 EndSphereCollision인지 확인
		if (OwningPlayer->GetGrabbedObject() == EndSphereCollision)
		{
			// EndSphereCollision의 현재 위치로 케이블의 끝 위치를 업데이트
			CableComponent->EndLocation = EndSphereCollision->GetComponentLocation();
			// 로그를 통해 업데이트된 위치를 확인
			UE_LOG( LogTemp , Warning , TEXT( "Cable end location updated to: %s" ) , *CableComponent->EndLocation.ToString() );
		}
	}
}


void ACable::HandleCableGrabbed(UPrimitiveComponent* HandMesh )
{
	FName SocketName = FName( TEXT( "hand_rPoint" ) ); // Name of the socket
	EndSphereCollision->AttachToComponent( HandMesh , FAttachmentTransformRules::SnapToTargetNotIncludingScale , SocketName );
}

void ACable::HandleCableReleased()
{
	EndSphereCollision->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform ); // 현재 부착된 컴포넌트에서 EndSphereCollision 분리
	CableComponent->EndLocation = NewEndSphereCollision->GetComponentLocation(); // 케이블의 끝 위치를 NewEndSphereCollision의 위치로 업데이트
}




