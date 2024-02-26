// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionKey.h"

// Sets default values
AInteractionKey::AInteractionKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 기본 루트 컴포넌트를 생성
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// 메시 컴포넌트를 생성하고 루트에 붙임.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);

	// 콜리전 박스 컴포넌트를 생성하고 루트에 붙임. 
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(RootComponent);
	boxComp->SetCollisionProfileName(TEXT("Trigger"));
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	boxComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	// 상호작용 감지 이벤트 바인딩
	boxComp->OnComponentBeginOverlap.AddDynamic(this,&AInteractionKey::OnInteractionBoxOverlapBegin);

	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractionKey::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractionKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractionKey::OnInteractionBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		// 머티리얼 에셋 로드
		UMaterialInterface* LoadedMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Engine/VREditor/BasicMeshes/MI_Ball_01.MI_Ball_01'")));

		if (LoadedMaterial)
		{
			// 메시 컴포넌트에 로드한 머티리얼 적용
			meshComp->SetMaterial(0, LoadedMaterial);
		}
	}
}

