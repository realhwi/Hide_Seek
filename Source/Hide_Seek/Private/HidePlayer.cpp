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
	LeftController->SetTrackingSource( EControllerHand::Left);
	// LeftController->SetTrackingMotionSource(FName("Left"));

	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Controller"));
	RightController->SetupAttachment(GetRootComponent());
	RightController->SetTrackingSource( EControllerHand::Right );
	// RightController->SetTrackingMotionSource(FName("Right"));

	//Hand Mesh
	LeftHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Hand Mesh"));
	LeftHandMesh->AttachToComponent( GetMesh() , FAttachmentTransformRules::SnapToTargetIncludingScale , TEXT( "hand_lPoint" ) );
	// LeftHandMesh->SetupAttachment(LeftController);

	RightHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Hand Mesh"));
	RightHandMesh->AttachToComponent( GetMesh() , FAttachmentTransformRules::SnapToTargetIncludingScale , TEXT( "hand_rPoint" ) );
	// RightHandMesh->SetupAttachment(RightController);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> LeftMeshFinder(TEXT("/Script/Engine.StaticMesh'/Game/JH/Models/left_OculusTouch_v2Controller.left_OculusTouch_v2Controller'"));
	if (LeftMeshFinder.Succeeded())
	{
		LeftHandMesh->SetStaticMesh(LeftMeshFinder.Object);
	}
	
	// Find and attach the static mesh for RightHandMesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RightMeshFinder(TEXT("/Script/Engine.StaticMesh'/Game/JH/Models/right_OculusTouch_v2Controller.right_OculusTouch_v2Controller'"));
	if (RightMeshFinder.Succeeded())
	{
		RightHandMesh->SetStaticMesh(RightMeshFinder.Object);
	}

	// LeftController 오버랩 이벤트 활성화
	LeftController->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
	LeftController->SetCollisionObjectType( ECC_GameTraceChannel1 ); // 사용자 정의 충돌 채널 설정
	LeftController->SetCollisionResponseToAllChannels( ECR_Ignore );
	LeftController->SetCollisionResponseToChannel( ECC_Pawn , ECR_Overlap );
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
	bIsTrigger = false;
}

// Called every frame
void AHidePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RightGrabbing();
	
	if (bIsTrigger)
	{
		PerformLineTrace();
	}
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
		InputSystem->BindAction(IA_Grab, ETriggerEvent::Started, this, &AHidePlayer::OnActionTryGrab );
		InputSystem->BindAction(IA_Grab, ETriggerEvent::Completed, this, &AHidePlayer::OnActionUnGrab );
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
	UE_LOG( LogTemp , Warning , TEXT( "Try" ) );

	FVector2D MovementVector = Value.Get<FVector2D>();

	// VR 플랫폼에서는 카메라의 방향을 기준으로 이동하므로, 카메라의 방향을 가져오기
	FVector ForwardVector = CameraComponent->GetForwardVector();
	FVector RightVector = CameraComponent->GetRightVector();

	// X 축은 카메라의 Forward 방향으로, Y 축은 카메라의 Right 방향으로 설정
	FVector MovementDirection = ForwardVector * MovementVector.Y + RightVector * MovementVector.X;

	// 이동 입력을 추가
	AddMovementInput(MovementDirection, 25.0f); 

	//PC 일때 사용
	/*AddMovementInput( GetActorForwardVector() , MovementVector.X );
	AddMovementInput( GetActorRightVector() , MovementVector.Y );*/

	//VR 일때 카메라 기준으로 해야함
	//예) GetComponentForward??
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

void AHidePlayer::OnActionTryGrab()
{
	UE_LOG( LogTemp , Warning , TEXT( "Try" ));
	FVector OverlapSphereCenter = RightController->GetComponentLocation();

	//Overlap Sphere와 겹친 물건을 저장할 Array
	TArray<FOverlapResult> HitObjects;
	FCollisionQueryParams CollisionParams;
	//무시할 Actor들
	CollisionParams.AddIgnoredActor( this );
	CollisionParams.AddIgnoredComponent( RightController );
	//Overlap 실행
	bool bIsHit = GetWorld()->OverlapMultiByChannel( HitObjects , OverlapSphereCenter , FQuat::Identity , ECC_Visibility , FCollisionShape::MakeSphere( GrabRange ) );

	//Grab bool 깃발 초기화	
	bIsGrabbed = false;

	//Overlap 결과가 없다면 밑에 코드 접근 불가
	if (!bIsHit) { return; }

	//가장 가까운 물건 Index
	int ClosestObjectIndex = 0;			//Index 변수 선언
	//가장 가까운 물건의 Index를 찾는 반복문
	for (int i = 0; i < HitObjects.Num(); ++i) {
		//Overlap와 겹친 물건이 Physics가 켜져이지 않다면 현재 Array Index 건너뛰기
		if (!HitObjects[i].GetComponent()->IsSimulatingPhysics()) { continue; }

		//Array에서 가장 가까운 물건 찾기 - HitObjects[ClosestObjectIndex]와 HitObjects[현재 Index] 비교함
		//저장한 Index와 손의 거리

		float DistBtwnHandClosest = FVector::Dist( OverlapSphereCenter , HitObjects[ClosestObjectIndex].GetComponent()->GetComponentLocation() );
		//현재 Index와 손의 거리
		float DistBtwnHandCurrent = FVector::Dist( OverlapSphereCenter , HitObjects[i].GetComponent()->GetComponentLocation() );

		//현재 Index가 더 가깝다면
		if (DistBtwnHandCurrent < DistBtwnHandClosest) {
			//현재 Index 저장하기	
			ClosestObjectIndex = i;
		}

		//Grab bool 깃발 올리기	
		bIsGrabbed = true;
	}

	//만약에 물건을 잡았고 Grab bool 깃발이 올라갔으면
	if (bIsGrabbed) {
		//ClosestObjectIndex를 사용해서 Grab한 물건을 HitObjects Array에서 찾아서 저장하기
		GrabbedObject = HitObjects[ClosestObjectIndex].GetComponent();
		//Physics 비활성화
		GrabbedObject->SetSimulatePhysics( false );
		//Collision 비활성화
		GrabbedObject->SetCollisionEnabled( ECollisionEnabled::NoCollision );
		///RightHand에 붙이기
		GrabbedObject->AttachToComponent( RightController , FAttachmentTransformRules::KeepWorldTransform );

		//잡기 위치 저장
		PreviousGrabRotation = RightController->GetComponentRotation().Quaternion();
		//잡기 회전값 저장
		PreviousGrabRotation = RightController->GetComponentQuat();
	}
}

void AHidePlayer::OnActionUnGrab()
{
	//이미 Grab 깃발이 내려가있다면 밑에 코드 접근 불가
	if (!bIsGrabbed) { return; }

	//Grab bool 깃발 내리기
	bIsGrabbed = false;
	//손에서 Attach한 Actor때기
	GrabbedObject->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform );
	//Physics 활성화하기
	GrabbedObject->SetSimulatePhysics( true );
	//Collision 활성화하기
	GrabbedObject->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );

	//던지기 힘 10%만 사용 
	float ReducedThrowStrength = ThrowStrength * 0.1f;
	GrabbedObject->AddForce( ThrowDirection * ThrowStrength * GrabbedObject->GetMass() );

	// 물건을 회전하기, 회전 10%만 사용
	float Angle; FVector Axis;
	//저장한 Quaternion인 Delta 회전값에서 Axis and Angle 추출하기
	DeltaRotation.ToAxisAndAngle( Axis , Angle );

	float ReducedTorquePower = TorquePower * 0.1f;
	FVector AngularVelocity = (1 / GetWorld()->DeltaTimeSeconds) * Angle * Axis;
	GrabbedObject->SetPhysicsAngularVelocityInRadians( AngularVelocity * ReducedTorquePower , true );
	
	// float DeltaTime = GetWorld()->DeltaTimeSeconds;
	//회전속도 계산하기
	// FVector AngularVelocity = (1 / DeltaTime) * Angle * Axis;
	//회전힘 적용하기
	// GrabbedObject->SetPhysicsAngularVelocityInRadians( AngularVelocity * TorquePower , true );

	//Grab한 물건을 놓았기 때문에 변수에 nullptr 할당 	
	GrabbedObject = nullptr;
}

void AHidePlayer::RightGrabbing()
{
	//Grab bool 깃발이 올라가지 않았다면 밑에 코드 접근 불가
	if (!bIsGrabbed) { return; }

	//던지기 방향 저장
	ThrowDirection = RightController->GetComponentLocation() - PreviousGrabPosition;

	//잡기 위치 저장
	PreviousGrabRotation = RightController->GetComponentRotation().Quaternion();

	//Quaternion 공식
	//angle1 = q1, angle2 = q2
	//angle1 + angle2 = q1 + q2
	//-angle1 = q1.inverse()
	//angle2 - angle1 = q2 * q1.inverse()

	//위에 공식 참고
	//회전값 계산하기
	DeltaRotation = RightController->GetComponentQuat() * PreviousGrabRotation.Inverse();
	//회전값 저장하기
	PreviousGrabRotation = RightController->GetComponentQuat();
}

void AHidePlayer::PerformLineTrace()
{
	if (!RightController) // RightHandMesh가 초기화되었는지 확인
	{
		UE_LOG( LogTemp , Warning , TEXT( "RightHandMesh is not initialized." ) );
		return;
	}

	FVector Start = RightController->GetComponentLocation(); // HandMesh 또는 HandController의 위치
	FVector ForwardVector = RightController->GetForwardVector(); // HandMesh 또는 HandController의 전방 벡터
	FVector End = Start + ForwardVector * 5000; // 선의 길이를 5000으로 조정
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor( this );

	if (GetWorld()->LineTraceSingleByChannel( HitResult , Start , End , ECC_Visibility , Params ))
	{
		DrawDebugLine( GetWorld() , Start , HitResult.ImpactPoint , FColor::Red , false , 0 , 0 , 1 );
	}
	else 
	{
		// 충돌이 없을 경우에도 선을 그리고 시각적으로 라인 트레이스를 확인 
		DrawDebugLine( GetWorld() , Start , End , FColor::Red , false , 0 , 0 , 1 );
	}
}

void AHidePlayer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// 오버랩이 시작됐을 때의 로직
	if (OtherActor && OtherActor->IsA( AInteraction::StaticClass() ))
	{
		bIsControllerOverlapping = true;
	}
}

void AHidePlayer::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	// 오버랩이 끝났을 때의 로직
	if (OtherActor && OtherActor->IsA( AInteraction::StaticClass() ))
	{
		bIsControllerOverlapping = false;
	}
}

void AHidePlayer::OnActionTrigger()
{
	UE_LOG( LogTemp , Warning , TEXT( "Trigger" ) );

	bIsTrigger = true;
}

void AHidePlayer::OnActionUnTrigger()
{
	UE_LOG( LogTemp , Warning , TEXT( "UnTrigger" ) );

	bIsTrigger = false;
}


