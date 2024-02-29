// Fill out your copyright notice in the Description page of Project Settings.


#include "HidePlayer.h"

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <InputActionValue.h>
#include "Engine/LocalPlayer.h"


AHidePlayer::AHidePlayer()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetRootComponent());
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->SetRelativeLocation(FVector(0,80,0));

}

void AHidePlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* Playercontroller =Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if(Playercontroller)
	{
		ULocalPlayer* LocalPlayer = Playercontroller->GetLocalPlayer();
		if(LocalPlayer)
		{
			UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( LocalPlayer );
			if (SubSystem)
			{
				SubSystem->AddMappingContext( IMC_VRInput , 0 );
			}
		}
	}
}

void AHidePlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AHidePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto InputSystem = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if(InputSystem)
	{
		InputSystem->BindAction(IA_Move,ETriggerEvent::Triggered,this,&AHidePlayer::Move);
		InputSystem->BindAction(IA_Look,ETriggerEvent::Triggered,this,&AHidePlayer::Look);

	}
}


void AHidePlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(),MovementVector.X);
	AddMovementInput(GetActorRightVector(),MovementVector.Y);

}

void AHidePlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}





