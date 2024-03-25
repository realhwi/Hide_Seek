// Fill out your copyright notice in the Description page of Project Settings.


#include "RedKey.h"

#include "PlayerUI.h"
#include "Components/Image.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/SphereComponent.h"

// Sets default values
ARedKey::ARedKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	redComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "redComp" ) );
	redComp->SetupAttachment( RootComponent );
	SphereComp = CreateDefaultSubobject<USphereComponent>( TEXT( "SphereComp" ) );
	SphereComp->SetupAttachment( redComp );
	SphereComp->SetSphereRadius( 30.f );
	SphereComp->SetCollisionProfileName( TEXT( "OverlapAllDynamic" ) );
}

// Called when the game starts or when spawned
void ARedKey::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic( this , &ARedKey::OnOverlapBegin );
}

// Called every frame
void ARedKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARedKey::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHidePlayer* HidePlayer = Cast<AHidePlayer>( OtherActor );
	if (!HidePlayer) {
		return;
	}
	UPlayerUI* playerUI = Cast<UPlayerUI>( HidePlayer->playerWidgetComp->GetUserWidgetObject() );
	if (playerUI && playerUI->red)
	{
		// Set the visibility of the hidden image directly to Visible
		playerUI->red->SetVisibility( ESlateVisibility::Visible );
	}
	Destroy();
}

