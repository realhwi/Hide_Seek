// Fill out your copyright notice in the Description page of Project Settings.


#include "HidePlayer.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <InputActionValue.h>
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/LocalPlayer.h"
#include "MotionControllerComponent.h"
#include "GameFramework/Actor.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Interaction.h"


// Sets default values
AHidePlayer::AHidePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//camera Comp
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetRootComponent());
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->SetRelativeLocation(FVector(0, 0, 90));

	//MotionController
	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Controller"));
	LeftController->SetupAttachment(GetRootComponent());
	LeftController->SetTrackingMotionSource(FName("Left"));

	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Controller"));
	RightController->SetupAttachment(GetRootComponent());
	RightController->SetTrackingMotionSource(FName("Right"));

	//Hand Mesh
	LeftHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Hand Mesh"));
	LeftHandMesh->SetupAttachment(LeftController);

	RightHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Hand Mesh"));
	RightHandMesh->SetupAttachment(RightController);

	LeftControllerCollision = CreateDefaultSubobject<USphereComponent>(TEXT("LefrControllerCollision"));
	LeftControllerCollision->SetupAttachment(LeftController);
	RightControllerCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RightControllerCollision"));
	RightControllerCollision->SetupAttachment(RightController);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> LeftMeshFinder(TEXT("/Game/JH/Models/left_OculusTouch_v2Controller.left_OculusTouch_v2Controller"));
	if (LeftMeshFinder.Succeeded())
	{
		LeftHandMesh->SetStaticMesh(LeftMeshFinder.Object);
		LeftHandMesh->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	}

	// Find and attach the static mesh for RightHandMesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RightMeshFinder(TEXT("/Game/JH/Models/right_OculusTouch_v2Controller.right_OculusTouch_v2Controller"));
	if (RightMeshFinder.Succeeded())
	{
		RightHandMesh->SetStaticMesh(RightMeshFinder.Object);
		RightHandMesh->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	}

	/*ConstructorHelpers::FObjectFinder<UStaticMesh>TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/JH/Models/left_OculusTouch_v2Controller.left_OculusTouch_v2Controller'"));
	if (TempMesh.Succeeded())
	{
		LeftHandMesh->SetStaticMesh(TempMesh.Object);
		LeftHandMesh->SetRelativeLocationAndRotation(FVector(-2.981260, -3.500000, 4.561753), FRotator(-25.000000, -179.999999, 89.999998));
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempMesh2(TEXT("/Script/Engine.StaticMesh'/Game/JH/Models/right_OculusTouch_v2Controller.right_OculusTouch_v2Controller'"));
	if (TempMesh2.Succeeded())
	{
		RightHandMesh->SetStaticMesh(TempMesh2.Object);
		RightHandMesh->SetRelativeLocationAndRotation(FVector(-2.981260, 3.500000, 4.561753), FRotator(25.000000, 0.000000, 89.999999));
	}*/

}

// Called when the game starts or when spawned
void AHidePlayer::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
	APlayerController* Playercontroller = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (Playercontroller)
	{
		ULocalPlayer* LocalPlayer = Playercontroller->GetLocalPlayer();
		if (LocalPlayer)
		{
			UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
			if (SubSystem)
			{
				SubSystem->AddMappingContext(IMC_JHVRInput, 0);
			}
		}
	}
}

// Called every frame
void AHidePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RightGrabbing();
	//TriggerGragging();
}

// Called to bind functionality to input
void AHidePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto InputSystem = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (InputSystem)
	{
		InputSystem->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AHidePlayer::Move);
		InputSystem->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AHidePlayer::Look);
		InputSystem->BindAction(IA_RightGrab, ETriggerEvent::Started, this, &AHidePlayer::OnActionTryRightGrab);
		InputSystem->BindAction(IA_RightGrab, ETriggerEvent::Completed, this, &AHidePlayer::OnActionUnRightGrab);
		InputSystem->BindAction(IA_Trigger, ETriggerEvent::Started, this, &AHidePlayer::OnActionTrigger);
		InputSystem->BindAction(IA_Trigger, ETriggerEvent::Completed, this, &AHidePlayer::OnActionUnTrigger);
	}
}

void AHidePlayer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AHidePlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// VR �÷��������� ī�޶��� ������ �������� �̵��ϹǷ�, ī�޶��� ������ ��������
	FVector ForwardVector = CameraComponent->GetForwardVector();
	FVector RightVector = CameraComponent->GetRightVector();

	// X ���� ī�޶��� Forward ��������, Y ���� ī�޶��� Right �������� ����
	FVector MovementDirection = ForwardVector * MovementVector.Y + RightVector * MovementVector.X;

	// �̵� �Է��� �߰�
	AddMovementInput(MovementDirection, 25.0f); 

	//PC �϶� ���
	/*AddMovementInput( GetActorForwardVector() , MovementVector.X );
	AddMovementInput( GetActorRightVector() , MovementVector.Y );*/

	//VR �϶� ī�޶� �������� �ؾ���
	//��) GetComponentForward??
	/*AddMovementInput(CameraComponent->GetForwardVector(), MovementVector.X);
	AddMovementInput(CameraComponent->GetRightVector(), MovementVector.Y);*/
}

void AHidePlayer::Look(const FInputActionValue& Value)
{

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		/*CameraComponent->bUsePawnControlRotation = true;*/

		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}

	UE_LOG(LogTemp, Warning, TEXT("Thumbstick Look: %s"), *LookAxisVector.ToString());

}

void AHidePlayer::OnActionTryRightGrab()
{
	FVector OverlapSphereCenter=RightController->GetComponentLocation();

	//Overlap Sphere�� ��ģ ������ ������ Array
	TArray<FOverlapResult> HitObjects;
	FCollisionQueryParams CollisionParams;
	//������ Actor��
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredComponent(RightController);
	//Overlap ����
	bool bIsHit = GetWorld()->OverlapMultiByChannel(HitObjects, OverlapSphereCenter, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(GrabRange));

	//Grab bool ��� �ʱ�ȭ	
	bIsGrabbed = false;

	//Overlap ����� ���ٸ� �ؿ� �ڵ� ���� �Ұ�
	if (!bIsHit) { return; }

	//���� ����� ���� Index
	int ClosestObjectIndex = 0;			//Index ���� ����
	//���� ����� ������ Index�� ã�� �ݺ���
	for (int i = 0; i < HitObjects.Num(); ++i) {
		//Overlap�� ��ģ ������ Physics�� �������� �ʴٸ� ���� Array Index �ǳʶٱ�
		if (!HitObjects[i].GetComponent()->IsSimulatingPhysics()) { continue; }

		//Array���� ���� ����� ���� ã�� - HitObjects[ClosestObjectIndex]�� HitObjects[���� Index] ����
		//������ Index�� ���� �Ÿ�

		float DistBtwnHandClosest = FVector::Dist(OverlapSphereCenter, HitObjects[ClosestObjectIndex].GetComponent()->GetComponentLocation());
		//���� Index�� ���� �Ÿ�
		float DistBtwnHandCurrent = FVector::Dist(OverlapSphereCenter, HitObjects[i].GetComponent()->GetComponentLocation());

		//���� Index�� �� �����ٸ�
		if (DistBtwnHandCurrent < DistBtwnHandClosest) {
			//���� Index �����ϱ�	
			ClosestObjectIndex = i;
		}

		//Grab bool ��� �ø���	
		bIsGrabbed = true;
	}

	//���࿡ ������ ��Ұ� Grab bool ����� �ö�����
	if (bIsGrabbed) {
		//ClosestObjectIndex�� ����ؼ� Grab�� ������ HitObjects Array���� ã�Ƽ� �����ϱ�
		GrabbedObject = HitObjects[ClosestObjectIndex].GetComponent();
		//Physics ��Ȱ��ȭ
		GrabbedObject->SetSimulatePhysics(false);
		//Collision ��Ȱ��ȭ
		GrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		///RightHand�� ���̱�
		GrabbedObject->AttachToComponent(RightController, FAttachmentTransformRules::KeepWorldTransform);

		//��� ��ġ ����
		PreviousGrabRotation = RightController->GetComponentRotation().Quaternion();
		//��� ȸ���� ����
		PreviousGrabRotation = RightController->GetComponentQuat();
	}
}

void AHidePlayer::OnActionUnRightGrab()
{
	//�̹� Grab ����� �������ִٸ� �ؿ� �ڵ� ���� �Ұ�
	if (!bIsGrabbed) { return; }

	//Grab bool ��� ������
	bIsGrabbed = false;
	//�տ��� Attach�� Actor����
	GrabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	//Physics Ȱ��ȭ�ϱ�
	GrabbedObject->SetSimulatePhysics(true);
	//Collision Ȱ��ȭ�ϱ�
	GrabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//������
	GrabbedObject->AddForce(ThrowDirection * ThrowStrength * GrabbedObject->GetMass());
	//������ ȸ���ϱ�
	float Angle;
	FVector Axis;
	//������ Quaternion�� Delta ȸ�������� Axis and Angle �����ϱ�
	DeltaRotation.ToAxisAndAngle(Axis, Angle);

	float DeltaTime = GetWorld()->DeltaTimeSeconds;
	//ȸ���ӵ� ����ϱ�
	FVector AngularVelocity = (1 / DeltaTime) * Angle * Axis;
	//ȸ���� �����ϱ�
	GrabbedObject->SetPhysicsAngularVelocityInRadians(AngularVelocity * TorquePower, true);

	//Grab�� ������ ���ұ� ������ ������ nullptr �Ҵ� 	
	GrabbedObject = nullptr;
}

void AHidePlayer::RightGrabbing()
{
	//Grab bool ����� �ö��� �ʾҴٸ� �ؿ� �ڵ� ���� �Ұ�
	if (!bIsGrabbed) { return; }

	//������ ���� ����
	ThrowDirection = RightController->GetComponentLocation() - PreviousGrabPosition;
	
	//��� ��ġ ����
	PreviousGrabRotation = RightController->GetComponentRotation().Quaternion();

	//Quaternion ����
	//angle1 = q1, angle2 = q2
	//angle1 + angle2 = q1 + q2
	//-angle1 = q1.inverse()
	//angle2 - angle1 = q2 * q1.inverse()

	//���� ���� ����
	//ȸ���� ����ϱ�
	DeltaRotation = RightController->GetComponentQuat() * PreviousGrabRotation.Inverse();
	//ȸ���� �����ϱ�
	PreviousGrabRotation = RightController->GetComponentQuat();
}

void AHidePlayer::TriggerGragging()
{
	//if (!bIsTriggered) { return; }
}

void AHidePlayer::OnActionTrigger()
{
	//FVector Start = LeftController->GetComponentLocation(); // �Ǵ� RightController, ��Ȳ�� ���� �ٸ�
	//FVector ForwardVector = LeftController->GetForwardVector(); // ��Ʈ�ѷ��� ���� ����
	//FVector End = Start + (ForwardVector * 1000); // 1000�� Line Trace�� ����
	//FHitResult HitResult;

	//bIsTriggered = true;
	//// Line Trace ����
	//if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	//{
	//	if (HitResult.GetActor() != nullptr)
	//	{
	//		// ���⼭ HitResult�� ����Ͽ� ��ȣ�ۿ� ���� ����
	//		ProcessInteraction(HitResult.GetActor());
	//	}
	//}
}

void AHidePlayer::OnActionUnTrigger()
{
	// bIsTriggered = false;
}

void AHidePlayer::ProcessInteraction(AActor* Actor)
{
	/*AInteraction* Interaction = Cast<AInteraction>(Actor);
	if(Interaction)
	{
		Interaction->Interact();
	}
	else
	{
		
	}*/
}




