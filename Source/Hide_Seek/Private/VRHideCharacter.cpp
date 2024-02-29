// Fill out your copyright notice in the Description page of Project Settings.


#include "VRHideCharacter.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <InputActionValue.h>
#include "Camera/CameraComponent.h"
#include "Engine/LocalPlayer.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"

// Sets default values
AVRHideCharacter::AVRHideCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>( TEXT( "CameraComponent" ) );
	CameraComponent->SetupAttachment( GetRootComponent() );
	CameraComponent->bUsePawnControlRotation = true;
	CameraComponent->SetRelativeLocation( FVector( 0 , 0 , 60 ) );

	LeftController = CreateDefaultSubobject<UMotionControllerComponent>( TEXT( "Left Controller" ) );
	LeftController->SetupAttachment( GetRootComponent() );
	LeftController->SetTrackingMotionSource( FName( "Left" ) );
	RightController = CreateDefaultSubobject<UMotionControllerComponent>( TEXT( "Right Controller" ) );
	RightController->SetupAttachment( GetRootComponent() );
	RightController->SetTrackingMotionSource( FName( "Right" ) );

	//Hand Mesh
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "Left Hand Mesh" ) );
	LeftHandMesh->SetupAttachment( LeftController );
	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "Right Hand Mesh" ) );

	RightHandMesh->SetupAttachment( RightController );

	/*ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh( TEXT(""));
	if (TempMesh.Succeeded()) 
	{
		LeftHandMesh->SetSkeletalMesh( TempMesh.Object );
		LeftHandMesh ->SetRelativeLocationAndRotation( FVector( -2.981260 , -3.500000 , 4.561753 ) ,FRotator( -25.000000 , -179.999999 , 89.999998 ) );
	}
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh2( TEXT(""));
	if (TempMesh2.Succeeded()) 
	{
		RightHandMesh->SetSkeletalMesh( TempMesh2.Object );
		RightHandMesh->SetRelativeLocationAndRotation( FVector( -2.981260 , 3.500000 , 4.561753 ) ,FRotator( 25.000000 , 0.000000 , 89.999999 ) );
	}*/
}

// Called when the game starts or when spawned
void AVRHideCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* Playercontroller = Cast<APlayerController>( GetWorld()->GetFirstPlayerController() );

	if (Playercontroller)
	{
		ULocalPlayer* LocalPlayer = Playercontroller->GetLocalPlayer();
		if (LocalPlayer)
		{
			UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( LocalPlayer );
			if (SubSystem)
			{
				SubSystem->AddMappingContext( IMC_JHVRInput , 0 );
			}
		}
	}
}

// Called every frame
void AVRHideCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRHideCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto InputSystem = CastChecked<UEnhancedInputComponent>( PlayerInputComponent );
	if (InputSystem)
	{
		InputSystem->BindAction( IA_Move , ETriggerEvent::Triggered , this , &AVRHideCharacter::Move );
		InputSystem->BindAction( IA_Look , ETriggerEvent::Triggered , this , &AVRHideCharacter::Look );

	}

}

void AVRHideCharacter::Move( const FInputActionValue& Value )
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	AddMovementInput( GetActorForwardVector() , MovementVector.X );
	AddMovementInput( GetActorRightVector() , MovementVector.Y );

}

void AVRHideCharacter::Look( const FInputActionValue& Value )
{
	FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerYawInput( LookVector.X );
	AddControllerPitchInput( LookVector.Y );

	UE_LOG(LogTemp, Warning, TEXT("%s"), *LookVector.ToString())
}

