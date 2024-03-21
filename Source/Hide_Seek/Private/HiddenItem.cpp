// Fill out your copyright notice in the Description page of Project Settings.


#include "HiddenItem.h"

#include "HidePlayer.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AHiddenItem::AHiddenItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HiddenComp = CreateDefaultSubobject<UNiagaraComponent>( TEXT( "HiddenComp" ) );
	HiddenComp->SetupAttachment( RootComponent );
	SphereComp = CreateDefaultSubobject<USphereComponent>( TEXT( "SphereComp" ) );
	SphereComp->SetupAttachment( HiddenComp );
	SphereComp->SetSphereRadius( 30.f ); 
	SphereComp->SetCollisionProfileName( TEXT( "OverlapAllDynamic" ) );

}

// Called when the game starts or when spawned
void AHiddenItem::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic( this , &AHiddenItem::OnOverlapBegin );
}

// Called every frame
void AHiddenItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHiddenItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHidePlayer* HidePlayer = Cast<AHidePlayer>( OtherActor );
	if (!HidePlayer){
		return;
	}

	//Server World의 내가 Overlap
	if(HidePlayer->HasAuthority())
	{
		HidePlayer->ServerChangeVisibility();
	}

	Destroy();
}

