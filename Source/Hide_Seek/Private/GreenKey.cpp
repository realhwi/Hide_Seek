// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenKey.h"

#include "PlayerUI.h"
#include "Components/Image.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/SphereComponent.h"
// Sets default values
AGreenKey::AGreenKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	greenComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "greenComp" ) );
	greenComp->SetupAttachment( RootComponent );
	SphereComp = CreateDefaultSubobject<USphereComponent>( TEXT( "SphereComp" ) );
	SphereComp->SetupAttachment( greenComp );
	SphereComp->SetSphereRadius( 30.f );
	SphereComp->SetCollisionProfileName( TEXT( "OverlapAllDynamic" ) );
}

// Called when the game starts or when spawned
void AGreenKey::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic( this , &AGreenKey::OnOverlapBegin );
}

// Called every frame
void AGreenKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGreenKey::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHidePlayer* HidePlayer = Cast<AHidePlayer>( OtherActor );
	if (!HidePlayer) 
	{
		return;
	}
	UPlayerUI* playerUI = Cast<UPlayerUI>( HidePlayer->playerWidgetComp->GetUserWidgetObject() );
	if (playerUI && playerUI->green)
	{
		// Set the visibility of the hidden image directly to Visible
		playerUI->green->SetVisibility( ESlateVisibility::Visible );
	}
	Destroy();
}

