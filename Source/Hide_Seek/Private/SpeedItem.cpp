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
	if (!bHasOverlapped)
	{
		if (AHidePlayer* HidePlayer = Cast<AHidePlayer>( OtherActor ))
		{
			if (AVREnemyPlayer* VREnemyPlayer = Cast<AVREnemyPlayer>( UGameplayStatics::GetPlayerCharacter( GetWorld() , 0 ) ))
			{
				UE_LOG( LogTemp , Warning , TEXT( "ChangeSpeed." ) );
				VREnemyPlayer->ChangeSpeed();
				bHasOverlapped = true;
			}
			UE_LOG( LogTemp , Warning , TEXT( "Overlap." ) );
			Destroy();
		}
	}
}

/*if(!HasAuthority())
			{
				UE_LOG( LogTemp , Warning , TEXT( "ChangeSpeed." ) );
				VREnemyPlayer->ChangeSpeed();
			}*/

/*
void ASpeedItem::RestorePlayerSpeed()
{
	if (AVREnemyPlayer* VREnemyPlayer = Cast<AVREnemyPlayer>( UGameplayStatics::GetPlayerCharacter( GetWorld() , 0 ) ))
	{
		float EnemySpeed = VREnemyPlayer->moveSpeed;
		UE_LOG( LogTemp , Warning , TEXT( "Speed restored." ) );
	}
}
*/


