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
	EndSphereCollision->SetupAttachment( RootComponent );
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

}

// Called when the game starts or when spawned
void ACable::BeginPlay()
{
	Super::BeginPlay();

	// 케이블 컴포넌트를 StartStaticMesh에 붙입니다.
	CableComponent->SetupAttachment( StartStaticMesh );
	// 케이블의 끝을 스피어 콜리전에 연결
	CableComponent->EndLocation = EndSphereCollision->GetComponentLocation();
}

// Called every frame
void ACable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACable::SetCableEndLocation(FVector NewLocation)
{
	// 케이블 컴포넌트의 EndLocation을 새 위치로 설정합니다.
	CableComponent->EndLocation = NewLocation;
}


