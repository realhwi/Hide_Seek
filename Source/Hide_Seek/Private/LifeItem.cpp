// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeItem.h"
#include "PlayerUI.h"
#include "HidePlayer.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
ALifeItem::ALifeItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LifeComp = CreateDefaultSubobject<UNiagaraComponent>( TEXT( "LifeComp" ) );
	LifeComp->SetupAttachment( RootComponent );
	SphereComp = CreateDefaultSubobject<USphereComponent>( TEXT( "SphereComp" ) );
	SphereComp->SetupAttachment( LifeComp );
	SphereComp->SetSphereRadius( 30.f ); // Set your desired radius
	SphereComp->SetCollisionProfileName( TEXT( "OverlapAllDynamic" ) );

}

// Called when the game starts or when spawned
void ALifeItem::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic( this , &ALifeItem::OnOverlapBegin );
}

// Called every frame
void ALifeItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALifeItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AHidePlayer* HidePlayer = Cast<AHidePlayer>( OtherActor ))
	{
		if (HidePlayer->LifeCount < HidePlayer->maxLifeCount)
		{
			HidePlayer->IncreaseLife();
			UE_LOG( LogTemp , Warning , TEXT( "Overlap." ) );
			Destroy();
		}
	}
}


