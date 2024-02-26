// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayer.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"

// Sets default values
AVRPlayer::AVRPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Character doesnt have a rifle at start
	bHasRifle = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = true;

	// ��ü ĳ���� �޽� ����
	GetMesh()->bCastDynamicShadow = true;
	GetMesh()->CastShadow = true;
	// �⺻ �޽ð� 1��Ī �������� ī�޶�� �Բ� ȸ���ϵ��� �մϴ�.
	GetMesh()->bOwnerNoSee = true;

	PitchValue = 0.0f;
	PitchSpeed = 1.0f;
}

// Called when the game starts or when spawned
void AVRPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void AVRPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FRotator ControlRotation = PC->GetControlRotation();

		// ī�޶��� Pitch ȸ�� ���� ��� ������ �����մϴ�. Ŭ������ �߰��մϴ�.
		PitchValue = FMath::Clamp(ControlRotation.Pitch, MinPitch, MaxPitch);
	}
}

void AVRPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}


void AVRPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Yaw ȸ�� �߰�
		AddControllerYawInput(LookAxisVector.X);

		// �����ӷ��� �������� Pitch ȸ�� ����
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		float PitchChange = LookAxisVector.Y * DeltaTime * PitchSpeed;
		PitchValue += PitchChange;

		// ������ Pitch ȸ������ ���� ���� ���� Ŭ����
		PitchValue = FMath::Clamp(PitchValue, MinPitch, MaxPitch);

		// ��Ʈ�ѷ��� Pitch �Է� �߰�
		AddControllerPitchInput(LookAxisVector.Y); 
	}
}


// Called to bind functionality to input
void AVRPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("LookUp", this, &AVRPlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AVRPlayer::AddControllerYawInput);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVRPlayer::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVRPlayer::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void AVRPlayer::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AVRPlayer::GetHasRifle()
{
	return bHasRifle;
}
