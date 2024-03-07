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
#include "PlayerUI.h"
#include "GameOver.h"
#include "VREnemyPlayer.h"
#include "Kismet/GameplayStatics.h"

class UWidgetComponent;
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

	GetMesh()->SetupAttachment( GetCapsuleComponent() );
	GetMesh()->SetRelativeLocation( FVector( 0.0f , 0.0f , 0.0f ) ); // 이 값은 실제 필요한 값으로 조정해야 합니다.

	//Hand Mesh
	LeftHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Hand Mesh"));
	// LeftHandMesh->AttachToComponent( GetMesh() , FAttachmentTransformRules::SnapToTargetIncludingScale , TEXT( "hand_lPoint" ) );
	LeftHandMesh->SetupAttachment(LeftController);

	RightHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Hand Mesh"));
	//RightHandMesh->AttachToComponent( GetMesh() , FAttachmentTransformRules::SnapToTargetIncludingScale , TEXT( "hand_rPoint" ) );
	RightHandMesh->SetupAttachment(RightController);

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

	// 캡슐 컴포넌트 콜리전 설정
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionResponseToChannel( ECollisionChannel::ECC_Pawn , ECollisionResponse::ECR_Overlap );

	/*if(GetMesh())
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshFinder( TEXT( "/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'" ) );
		GetMesh()->SetSkeletalMesh( MeshFinder.Object);
	}*/
}


// Called when the game starts or when spawned
void AHidePlayer::BeginPlay()
{
	Super::BeginPlay();

	
	APlayerController* Playercontroller = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	// 위젯 인스턴스 생성 및 화면에 추가
	if (playerUIFactory)
	{
		playerUI = CreateWidget<UPlayerUI>( GetWorld() , playerUIFactory );
		if (playerUI)
		{
			playerUI->AddToPlayerScreen();

			// 초기 생명값 설정을 위해 AddLife() 호출
			for (int32 i = 0; i < maxLifeCount; i++)
			{
				playerUI->AddLife();
			}
		}
	}
	if (playerUI)
	{
		playerUI->OnLifeDepleted.AddDynamic( this , &AHidePlayer::OnLifeDepleted );
	}


	// 오버랩 이벤트 핸들러를 바인딩
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic( this , &AHidePlayer::OnOverlapBegin );

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
	// 바닥에서 올라와서 걷는 거 
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin( EHMDTrackingOrigin::Floor );
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

bool AHidePlayer::IsTrigger() const
{
	return bIsTrigger;
}

bool AHidePlayer::IsGrab() const
{
	return bIsGrabbed;
}

void AHidePlayer::OnTriggerInteract(AInteraction* InteractionActor)
{
	InteractionActor->OnTriggerInteract( this );
}

void AHidePlayer::OnGrabInteract(AInteraction* InteractionActor)
{
	InteractionActor->OnGrabInteract( this );
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
	if (bIsGrabbed) 
	{
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

		if (GrabbedObject)
		{
			UE_LOG( LogTemp , Warning , TEXT( "GrabbedObject" ) );

			// 잡고 있는 객체가 AInteraction 클래스의 인스턴스인지 확인
			AInteraction* InteractionActor = Cast<AInteraction>( GrabbedObject->GetOwner() );
			if (InteractionActor)
			{
				UE_LOG( LogTemp , Warning , TEXT( "Grap!!" ) );
				// AInteraction 인스턴스에 대한 추가 작업 수행
				OnTriggerInteract( InteractionActor );
			}
		}
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
	float ReducedThrowStrength = ThrowStrength * 1.0f;
	GrabbedObject->AddForce( ThrowDirection * ThrowStrength * GrabbedObject->GetMass() );

	// 물건을 회전하기, 회전 10%만 사용
	float Angle; FVector Axis;
	//저장한 Quaternion인 Delta 회전값에서 Axis and Angle 추출하기
	DeltaRotation.ToAxisAndAngle( Axis , Angle );

	float ReducedTorquePower = TorquePower * 1.0f;
	FVector AngularVelocity = Axis * FMath::DegreesToRadians( Angle ) / GetWorld()->DeltaTimeSeconds;
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
	FVector DownVector = -RightController->GetUpVector(); // 컨트롤러의 위 방향의 반대
	// ForwardVector와 DownVector를 조합하여 라인트레이스의 방향을 아래로
	// 여기서는 ForwardVector의 90%와 DownVector의 10%를 조합
	FVector Direction = ForwardVector * 0.9f + DownVector * 0.1f;
	Direction.Normalize(); // 방향 벡터를 정규화합니다.
	// 선의 길이를 5000으로 조정
	FVector End = Start + ForwardVector * 1000; 
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor( this );

	if (GetWorld()->LineTraceSingleByChannel( HitResult , Start , End , ECC_Visibility , Params ))
	{
		DrawDebugLine( GetWorld() , Start , HitResult.ImpactPoint , FColor::Red , false , 0 , 0 , 0.1 );
		
		// 여기서 충돌된 액터가 AInteraction 클래스의 인스턴스인지 확인
		AInteraction* InteractionActor = Cast<AInteraction>( HitResult.GetActor() );
		if (InteractionActor)
		{
			// 인터렉션 액터와 상호작용 처리를 수행하는 함수를 호출
			OnTriggerInteract( InteractionActor );
		}
	}
	else
	{
		DrawDebugLine( GetWorld() , Start , End , FColor::Red , false , 0 , 0 , 0.1 );
	}
}

void AHidePlayer::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherActor->IsA( AVREnemyPlayer::StaticClass() ))
	{
		UE_LOG( LogTemp , Warning , TEXT( "Overlapped!!" ) );
		// count가 0 이하라면 함수 종료 
		if(LifeCount<=0)
			return;
		// 생명 카운트 다운 
		LifeCount--;
		if(playerUI)
		{
			playerUI->RemoveLife( LifeCount );
		}

		if (bFromSweep) // 스위핑 중에 충돌이 발생한 경우에만
		{
			// 충돌 위치에 파티클 시스템을 생성
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld() ,
				VFX , // VFX는 유효한 UParticleSystem* 참조
				SweepResult.ImpactPoint , // 충돌 지점에서 에미터를 생성
				SweepResult.ImpactNormal.Rotation() // 에미터의 회전은 충돌 표면의 법선을 기준으로 설정
				);
			bLifeRemove = true;
		}
	}
}

void AHidePlayer::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	bLifeRemove = false;
}

void AHidePlayer::OnLifeDepleted()
{
	if (GameOverUIFactory != nullptr)
	{
		GameOverUI = CreateWidget<UGameOver>( GetWorld() , GameOverUIFactory );
		if (GameOverUI != nullptr)
		{
			GameOverUI->AddToViewport();
		}
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


