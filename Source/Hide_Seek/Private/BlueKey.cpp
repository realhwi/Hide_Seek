// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueKey.h"

#include "PlayerUI.h"
#include "Components/Image.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/SphereComponent.h"

// Sets default values
ABlueKey::ABlueKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	blueComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "blueComp" ) );
	blueComp->SetupAttachment( RootComponent );
	SphereComp = CreateDefaultSubobject<USphereComponent>( TEXT( "SphereComp" ) );
	SphereComp->SetupAttachment( blueComp );
	SphereComp->SetSphereRadius( 30.f );
	SphereComp->SetCollisionProfileName( TEXT( "OverlapAllDynamic" ) );
}

// Called when the game starts or when spawned
void ABlueKey::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic( this , &ABlueKey::OnOverlapBegin );
}

// Called every frame
void ABlueKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlueKey::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHidePlayer* HidePlayer = Cast<AHidePlayer>( OtherActor );
	if (!HidePlayer)
	{
		return;
	}
	UPlayerUI* playerUI = Cast<UPlayerUI>( HidePlayer->playerWidgetComp->GetUserWidgetObject() );
	if (playerUI && playerUI->blue)
	{
		// Set the visibility of the hidden image directly to Visible
		playerUI->blue->SetVisibility( ESlateVisibility::Visible );
	}
	Destroy();
}

