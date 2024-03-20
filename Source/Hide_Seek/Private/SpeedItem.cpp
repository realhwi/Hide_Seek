// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedItem.h"
#include "HiddenItem.h"
#include "HidePlayer.h"
#include "NiagaraComponent.h"
#include "VREnemyPlayer.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpeedItem::ASpeedItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpeedComp = CreateDefaultSubobject<UNiagaraComponent>( TEXT( "SpeedComp" ) );
	SpeedComp->SetupAttachment( RootComponent );
	SphereComp = CreateDefaultSubobject<USphereComponent>( TEXT( "SphereComp" ) );
	SphereComp->SetupAttachment( SpeedComp );
	SphereComp->SetSphereRadius( 30.f );
	SphereComp->SetCollisionProfileName( TEXT( "OverlapAllDynamic" ) );

}

// Called when the game starts or when spawned
void ASpeedItem::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic( this , &ASpeedItem::OnOverlapBegin );
}

// Called every frame
void ASpeedItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpeedItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AHidePlayer* HidePlayer = Cast<AHidePlayer>( OtherActor ))
	{
		if (AVREnemyPlayer* VREnemyPlayer = Cast<AVREnemyPlayer>( UGameplayStatics::GetPlayerCharacter( GetWorld() , 0 ) ))
		{
			float EnemySpeed = VREnemyPlayer->moveSpeed;

			// 획득 후 일정 시간이 지나면 스피드를 원래대로 복원하기 위해 타이머 설정
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer( TimerHandle , this , &ASpeedItem::RestorePlayerSpeed , 10.0f , false );

			UE_LOG( LogTemp , Warning , TEXT( "Item picked up. Speed Down" ) );
		}

		//auto pc = Cast<AVREnemyPlayer>()
		UE_LOG( LogTemp , Warning , TEXT( "Overlap." ) );
		Destroy();
	}
}

void ASpeedItem::RestorePlayerSpeed()
{
	if (AVREnemyPlayer* VREnemyPlayer = Cast<AVREnemyPlayer>( UGameplayStatics::GetPlayerCharacter( GetWorld() , 0 ) ))
	{
		float EnemySpeed = VREnemyPlayer->moveSpeed;
		UE_LOG( LogTemp , Warning , TEXT( "Speed restored." ) );
	}
}


