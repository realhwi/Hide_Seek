// Fill out your copyright notice in the Description page of Project Settings.


#include "VREnemyPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AVREnemyPlayer::AVREnemyPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(GetRootComponent());
	cameraComp->bUsePawnControlRotation = true;
	cameraComp->SetRelativeLocation(FVector(0, 0, 60));

	leftMotionCtrl = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionCtrl"));
	leftMotionCtrl->SetupAttachment(GetRootComponent());
	leftMotionCtrl->SetTrackingMotionSource(FName("Left"));

	rightMotionCtrl = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionCtrl"));
	rightMotionCtrl->SetupAttachment(GetRootComponent());
	rightMotionCtrl->SetTrackingMotionSource(FName("Right"));

	leftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	leftHandMesh->SetupAttachment(leftMotionCtrl);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempLeftHandMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_QuinnXR_left.SKM_QuinnXR_left'"));

	if (tempLeftHandMesh.Succeeded())
	{
		leftHandMesh->SetSkeletalMesh(tempLeftHandMesh.Object);
		leftHandMesh->SetRelativeLocationAndRotation(FVector(-3, -3.5, 4.5), FRotator(-25, -180, 90));
	}

	rightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	rightHandMesh->SetupAttachment(rightMotionCtrl);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempRightHandMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_QuinnXR_right.SKM_QuinnXR_right'"));

	if (tempRightHandMesh.Succeeded())
	{
		rightHandMesh->SetSkeletalMesh(tempRightHandMesh.Object);
		rightHandMesh->SetRelativeLocationAndRotation(FVector(-3, 3.5, 4.5), FRotator(25, 0, 90));
	}
}

// Called when the game starts or when spawned
void AVREnemyPlayer::BeginPlay()
{
	Super::BeginPlay();

	//UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);

	APlayerController* playerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if(playerController)
	{
		ULocalPlayer* localPlayer = playerController->GetLocalPlayer();
		if (localPlayer)
		{
			UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer);
			if(subSystem)
			{
				subSystem->AddMappingContext(IMC_VREnemyInput, 0);
			}
		}
	}
}

// Called every frame
void AVREnemyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVREnemyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto inputSystem = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (inputSystem)
	{
		inputSystem->BindAction(IA_VREnemyMove, ETriggerEvent::Triggered, this, &AVREnemyPlayer::EnemyMove);
		inputSystem->BindAction(IA_VREnemyLook, ETriggerEvent::Triggered, this, &AVREnemyPlayer::EnemyLook);

		//inputSystem->BindAction(IA_EnemyInteraction, ETriggerEvent::Started, this, &AVREnemyPlayer::EInteractionStart);
		//inputSystem->BindAction(IA_EnemyInteraction, ETriggerEvent::Ongoing, this, &AVREnemyPlayer::EInteractionOnGoing);
		//inputSystem->BindAction(IA_EnemyInteraction, ETriggerEvent::Completed, this, &AVREnemyPlayer::EInteractionComplete);
		
	}
}

void AVREnemyPlayer::EnemyMove(const FInputActionValue& value)
{
	FVector2D movementVector = value.Get<FVector2D>();

	AddMovementInput(cameraComp->GetForwardVector(), movementVector.Y);
	AddMovementInput(cameraComp->GetRightVector(), movementVector.X);
}

void AVREnemyPlayer::EnemyLook(const FInputActionValue& value)
{
	FVector2D lookVector = value.Get<FVector2D>();

	if(nullptr != Controller)
	{
		AddControllerYawInput(lookVector.X);
		AddControllerPitchInput(-lookVector.Y);
	}
}

//void AVREnemyPlayer::EInteractionStart(const FInputActionValue& value)
//{
//	moveSpeed = 400;
//
//	FVector loc = GetActorLocation() + FVector( 0 , 100 , 0 );
//
//	DrawDebugString( GetWorld() , loc , TEXT( "Player" ) , this , FColor::Black , 5 , false , 1 );
//}
//
//void AVREnemyPlayer::EInteractionOnGoing(const FInputActionValue& value)
//{
//	TArray<struct FOverlapResult> outOverlaps;
//	// FVector pos = rightMotionCtrl->GetComponentLocation();
//	FVector pos = GetActorLocation();
//	FQuat rot = FQuat::Identity;
//	FCollisionObjectQueryParams objectQueryParams( FCollisionObjectQueryParams::InitType::AllObjects );
//	FCollisionShape collisionShape = FCollisionShape::MakeSphere(findActorRadius);
//
//	bool bHits = GetWorld()->OverlapMultiByObjectType( outOverlaps , pos , rot , objectQueryParams , collisionShape );
//
//	if (bHits)
//	{
//		for(auto result : outOverlaps)
//		{
//			if (result.GetActor()->ActorHasTag( TEXT( "Player" ) ))
//			{
//				playerChar = result.GetActor();
//
//				break;
//			}
//		}
//	}
//
//	if(playerChar)
//	{
//		FVector loc = GetActorLocation() + FVector(0 , 100 , 0 );
//
//		DrawDebugString( GetWorld() , loc , TEXT( "Player" ) , nullptr , FColor::Black , 0 , false , 1 );
//	}
//
//	DrawDebugSphere( GetWorld() , GetActorLocation() , findActorRadius , 16 , FColor::Cyan , false , 0 );
//}
//
//void AVREnemyPlayer::EInteractionComplete(const FInputActionValue& value)
//{
//	moveSpeed = 600;
//}

