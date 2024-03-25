// Fill out your copyright notice in the Description page of Project Settings.


#include "VREnemyPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "VRGameStateBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AVREnemyPlayer::AVREnemyPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize( 42.0f , 80.0f );

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(GetRootComponent());
	cameraComp->bUsePawnControlRotation = true;
	cameraComp->SetRelativeLocation(FVector(0, 0, 60));

	leftMotionCtrl = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionCtrl"));
	leftMotionCtrl->SetupAttachment(GetRootComponent());
	leftMotionCtrl->SetTrackingSource( EControllerHand::Left );
	//leftMotionCtrl->SetTrackingMotionSource(FName("Left"));

	rightMotionCtrl = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionCtrl"));
	rightMotionCtrl->SetupAttachment(GetRootComponent());
	rightMotionCtrl->SetTrackingSource( EControllerHand::Right );
	//rightMotionCtrl->SetTrackingMotionSource(FName("Right"));

	//leftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	//leftHandMesh->SetupAttachment(leftMotionCtrl);

	//ConstructorHelpers::FObjectFinder<USkeletalMesh> tempLeftHandMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_QuinnXR_left.SKM_QuinnXR_left'"));

	//if (tempLeftHandMesh.Succeeded())
	//{
	//	leftHandMesh->SetSkeletalMesh(tempLeftHandMesh.Object);
	//	//leftHandMesh->SetRelativeLocationAndRotation(FVector(-3, -3.5, 4.5), FRotator(-25, -180, 90));
	//	leftHandMesh->SetRelativeLocationAndRotation( FVector( -3 , -3.5 , 4.5 ) , FRotator(  0, 0 ,0  ) );
	//}

	//rightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	//rightHandMesh->SetupAttachment(rightMotionCtrl);

	//ConstructorHelpers::FObjectFinder<USkeletalMesh> tempRightHandMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_QuinnXR_right.SKM_QuinnXR_right'"));

	//if (tempRightHandMesh.Succeeded())
	//{
	//	rightHandMesh->SetSkeletalMesh(tempRightHandMesh.Object);
	//	// rightHandMesh->SetRelativeLocationAndRotation(FVector(-3, 3.5, 4.5), FRotator(25, 0, 90));
	//}

	handSphereColl = CreateDefaultSubobject<USphereComponent>( TEXT( "HandSphereColl" ) );
	handSphereColl->SetupAttachment( GetMesh() );
	handSphereColl->AttachToComponent( GetMesh() , FAttachmentTransformRules::SnapToTargetIncludingScale , TEXT( "EnemyRHPoint" ) );
	// handSphereColl->SetRelativeLocation( FVector( -10 , 0 , 0 ) );
	handSphereColl->SetSphereRadius( 10.f );
	handSphereColl->ComponentTags.Add( FName( "EnemyHand" ) );

	bReplicates = true;
}

// Called when the game starts or when spawned
void AVREnemyPlayer::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

	//UE_LOG( LogTemp , Warning , TEXT( "beginplay" ) )

	handSphereColl->SetRelativeLocation( FVector( -10 , 0 , 0 ) );

	GetMesh()->SetScalarParameterValueOnMaterials( TEXT( "Power" ) , 10 );

	vrGameStateBase = Cast<AVRGameStateBase>( GetWorld()->GetGameState() );

	bHasController = false;

}

// Called every frame
void AVREnemyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bHasController)
	{

		APlayerController* playerController = Cast<APlayerController>( GetWorld()->GetFirstPlayerController() );
		if (playerController)
		{
			ULocalPlayer* localPlayer = playerController->GetLocalPlayer();
			if (localPlayer)
			{
				UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( localPlayer );
				if (subSystem)
				{
					subSystem->AddMappingContext( IMC_VREnemyInput , 0 );
					//bHasController = true;
					//UE_LOG( LogTemp , Warning , TEXT( "has controller" ))

				}
			}
		}
	}
	FirstSkillActive();
	SecondSkillActive();
	ThirdSkillActive();

	ServerRPC_EnemyPlayerWin();
}

// Called to bind functionality to input
void AVREnemyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto inputSystem = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (inputSystem)
	{
		// InputComponents = inputSystem;
		inputSystem->BindAction(IA_VREnemyMove, ETriggerEvent::Triggered, this, &AVREnemyPlayer::EnemyMove);
		inputSystem->BindAction(IA_VREnemyLook, ETriggerEvent::Triggered, this, &AVREnemyPlayer::EnemyLook);

		inputSystem->BindAction(IA_EnemyInteraction, ETriggerEvent::Started, this, &AVREnemyPlayer::EInteractionStart);
		//inputSystem->BindAction(IA_EnemyInteraction, ETriggerEvent::Triggered, this, &AVREnemyPlayer::EInteractionOnGoing);
		inputSystem->BindAction(IA_EnemyInteraction, ETriggerEvent::Completed, this, &AVREnemyPlayer::EInteractionComplete);
	}
}

void AVREnemyPlayer::ChangeSpeed()
{
	UE_LOG( LogTemp , Warning , TEXT( "AVREnemyPlayer::ServerChangeSpeed_Implementation." ) );

	GetCharacterMovement()->MaxWalkSpeed = 100;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer( TimerHandle , this , &AVREnemyPlayer::RestorePlayerSpeed , 10.0f , false );
}


void AVREnemyPlayer::RestorePlayerSpeed()
{
	//moveSpeed 기본값으로 초기화
	GetCharacterMovement()->MaxWalkSpeed = 600;

	UE_LOG( LogTemp , Warning , TEXT( "Speed restored." ) );
}


void AVREnemyPlayer::EnemyMove(const FInputActionValue& value)
{
	FVector2D movementVector = value.Get<FVector2D>();

	FVector ForwardVector = cameraComp->GetForwardVector();
	FVector RightVector = cameraComp->GetRightVector();

	FVector movementDirection = ForwardVector * movementVector.Y + RightVector * movementVector.X;

	AddMovementInput( movementDirection , 25.0f );
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

void AVREnemyPlayer::EInteractionStart(const FInputActionValue& value)
{
	ServerRPC_ActionHandUp();
}

void AVREnemyPlayer::EInteractionComplete(const FInputActionValue& value)
{
	ServerRPC_ActionHandDown();
}

void AVREnemyPlayer::FirstSkillActive()
{
	if (activeSkillTime == 60 && activeFirstSkill == false)
	{
		GetMesh()->SetScalarParameterValueOnMaterials( TEXT( "Power" ) , 7 );
		ServerRPC_ItemBoxLock();
		activeFirstSkill = true;
	}
}

void AVREnemyPlayer::SecondSkillActive()
{
	if(activeSkillTime == 120 && activeSecondSkill == false)
	{
		GetMesh()->SetScalarParameterValueOnMaterials( TEXT( "Power" ) , 5);
		ServerRPC_ElectricBoxLock();
		activeSecondSkill = true;
	}
}

void AVREnemyPlayer::ThirdSkillActive()
{
	if (activeSkillTime == 180 && activeThirdSkill == false)
	{
		GetMesh()->SetScalarParameterValueOnMaterials( TEXT( "Power" ) , 3 );
		ServerRPC_EscapeLock();
		activeThirdSkill = true;
	}
	
}

//void AVREnemyPlayer::EInteractionStart(const FInputActionValue& value)
//{
//	moveSpeed = 400;
//
//	FVector loc = GetActorLocation() + FVector( 0 , 100 , 0 );
//	DrawDebugString( GetWorld() , loc , TEXT( "Player" ) , this , FColor::Black , 5 , false , 1 );
//}
//
//void AVREnemyPlayer::EInteractionTriggered(const FInputActionValue& value)
//{
//	TArray<struct FOverlapResult> outOverlaps;
//	//FVector pos = rightMotionCtrl->GetComponentLocation();
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

void AVREnemyPlayer::ServerRPC_ActionHandUp_Implementation()
{
	isHandUP = true;
	canCheckActor = true;
}

void AVREnemyPlayer::ServerRPC_ActionHandDown_Implementation()
{
	isHandUP = false;
	canCheckActor = false;
}


void AVREnemyPlayer::ServerRPC_ItemBoxLock_Implementation()
{
	canItemBoxLock = true;
}

void AVREnemyPlayer::ServerRPC_ElectricBoxLock_Implementation()
{
	canElectricBoxLock = true;
}

void AVREnemyPlayer::ServerRPC_EscapeLock_Implementation()
{
	canEscapeLock = true;
}

//void AVREnemyPlayer::ServerRPC_EnemyPlayerWin_Implementation()
//{
//	// auto vrGameStateBase = Cast<AVRGameStateBase>(GetWorld()->GetGameState());
//
//	if(setAltarOnChip >= 1)
//	{
//		isAltarActivate = true;
//	}
//
//	if (vrGameStateBase->isTimeOut == true)
//	{
//		isEnemyWin = true;
//	}
//}

void AVREnemyPlayer::ServerRPC_EnemyPlayerWin_Implementation()
{
	if(isAltarActivate == true || setAltarOnChip >= 3)

	isEnemyWin = true;
}

void AVREnemyPlayer::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AVREnemyPlayer , isHandUP )
	DOREPLIFETIME( AVREnemyPlayer , canCheckActor )
	DOREPLIFETIME( AVREnemyPlayer , countHPChip )
	DOREPLIFETIME( AVREnemyPlayer , canItemBoxLock )
	DOREPLIFETIME( AVREnemyPlayer , canElectricBoxLock )
	DOREPLIFETIME( AVREnemyPlayer , canEscapeLock )
	DOREPLIFETIME( AVREnemyPlayer , isAltarActivate )
	DOREPLIFETIME( AVREnemyPlayer , isPraying )
	DOREPLIFETIME(AVREnemyPlayer, isEnemyWin)
	DOREPLIFETIME( AVREnemyPlayer , isEnemyLose )
}
