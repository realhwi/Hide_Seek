// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionKey.h"

// Sets default values
AInteractionKey::AInteractionKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �⺻ ��Ʈ ������Ʈ�� ����
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// �޽� ������Ʈ�� �����ϰ� ��Ʈ�� ����.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);

	// �ݸ��� �ڽ� ������Ʈ�� �����ϰ� ��Ʈ�� ����. 
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(RootComponent);
	boxComp->SetCollisionProfileName(TEXT("Trigger"));
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	boxComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	// ��ȣ�ۿ� ���� �̺�Ʈ ���ε�
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
		// ��Ƽ���� ���� �ε�
		UMaterialInterface* LoadedMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Engine/VREditor/BasicMeshes/MI_Ball_01.MI_Ball_01'")));

		if (LoadedMaterial)
		{
			// �޽� ������Ʈ�� �ε��� ��Ƽ���� ����
			meshComp->SetMaterial(0, LoadedMaterial);
		}
	}
}

