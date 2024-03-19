// Fill out your copyright notice in the Description page of Project Settings.


#include "HidePlayer.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <InputActionValue.h>
#include "Cable.h"
#include "CableComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/LocalPlayer.h"
#include "MotionControllerComponent.h"
#include "GameFramework/Actor.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "PlayerUI.h"
#include "GameOver.h"
#include "VREnemyPlayer.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

class UWidgetComponent;
// Sets default values
AHidePlayer::AHidePlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize( 42.f , 96.0f );

	//camera Comp
	CameraComponent = CreateDefaultSubobject<UCameraComponent>( TEXT( "CameraComponent" ) );
	CameraComponent->SetupAttachment( GetRootComponent() );
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->Mobility = EComponentMobility::Movable;
	CameraComponent->SetRelativeLocation( FVector( 0 , 0 , 90 ) );

	//MotionController
	LeftController = CreateDefaultSubobject<UMotionControllerComponent>( TEXT( "Left Controller" ) );
	LeftController->SetupAttachment( GetRootComponent() );
	LeftController->SetTrackingSource( EControllerHand::Left );
	// LeftController->SetTrackingMotionSource(FName("Left"));

	RightController = CreateDefaultSubobject<UMotionControllerComponent>( TEXT( "Right Controller" ) );
	RightController->SetupAttachment( GetRootComponent() );
	RightController->SetTrackingSource( EControllerHand::Right );
	// RightController->SetTrackingMotionSource(FName("Right"));

	GetMesh()->SetupAttachment( GetCapsuleComponent() );
	GetMesh()->SetRelativeLocation( FVector( 0.0f , 0.0f , 0.0f ) ); // 이 값은 실제 필요한 값으로 조정해야 합니다.

	//Hand Mesh
	LeftHandMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Left Hand Mesh" ) );
	LeftHandMesh->SetupAttachment( GetMesh() , TEXT( "hand_lPoint" ) );
	// LeftHandMesh->SetupAttachment(LeftController);

	RightHandMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Right Hand Mesh" ) );
	RightHandMesh->SetupAttachment( GetMesh() , TEXT( "hand_rPoint" ) );
	// RightHandMesh->SetupAttachment(RightController);

	RightHandMesh->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
	RightHandMesh->SetCollisionObjectType( ECC_GameTraceChannel2 ); // You can define your own game trace channel
	RightHandMesh->SetCollisionResponseToAllChannels( ECR_Ignore );
	RightHandMesh->SetCollisionResponseToChannel( ECC_PhysicsBody , ECR_Overlap ); // Respond to overlap events with physics bodies

	//RightHandMesh->OnComponentBeginOverlap.AddDynamic( this , &AHidePlayer::OnHandMeshOverlapBegin );

	// LeftController 오버랩 이벤트 활성화
	LeftController->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
	LeftController->SetCollisionObjectType( ECC_GameTraceChannel1 ); // 사용자 정의 충돌 채널 설정
	LeftController->SetCollisionResponseToAllChannels( ECR_Ignore );
	LeftController->SetCollisionResponseToChannel( ECC_Pawn , ECR_Overlap );

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> LeftMeshFinder( TEXT( "/Script/Engine.StaticMesh'/Game/JH/Models/left_OculusTouch_v2Controller.left_OculusTouch_v2Controller'" ) );
	//if (LeftMeshFinder.Succeeded())
	//{
	//	LeftHandMesh->SetStaticMesh( LeftMeshFinder.Object );
	//}

	//// Find and attach the static mesh for RightHandMesh
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> RightMeshFinder( TEXT( "/Script/Engine.StaticMesh'/Game/JH/Models/right_OculusTouch_v2Controller.right_OculusTouch_v2Controller'" ) );
	//if (RightMeshFinder.Succeeded())
	//{
	//	RightHandMesh->SetStaticMesh( RightMeshFinder.Object );
	//}

	/*if(GetMesh())
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshFinder( TEXT( "/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'" ) );
		GetMesh()->SetSkeletalMesh( MeshFinder.Object);
	}*/

	CableActor = nullptr; // 초기화
	bIsGrabbed = true;
}


// Called when the game starts or when spawned
void AHidePlayer::BeginPlay()
{
	Super::BeginPlay();

	// 캡슐 컴포넌트 콜리전 설정
	GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
	GetCapsuleComponent()->SetCollisionResponseToChannel( ECollisionChannel::ECC_Pawn , ECollisionResponse::ECR_Overlap );

	APlayerController* Playercontroller = Cast<APlayerController>( GetWorld()->GetFirstPlayerController() );

	// 위젯 인스턴스 생성 및 화면에 추가
	if (playerUIFactory)
	{
		playerUI = CreateWidget<UPlayerUI>( GetWorld() , playerUIFactory );
		if (playerUI)
		{
			playerUI->AddToViewport();
			for (int32 i = 0; i < LifeCount; i++)
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
			UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( LocalPlayer );
			if (SubSystem)
			{
				SubSystem->AddMappingContext( IMC_JHVRInput , 0 );
			}
		}
	}
	// 바닥에서 올라와서 걷는 거 
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin( EHMDTrackingOrigin::Floor );

	// 플레이어 메시에 접근해서 머터리얼 저장
	UMeshComponent* PlayerMesh = GetMesh();
	OriginalMaterial1 = PlayerMesh->GetMaterial( 0 );
	OriginalMaterial2 = PlayerMesh->GetMaterial( 1 );
}

// Called every frame
void AHidePlayer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	RightGrabbing();

	if (bIsTrigger)
	{
		PerformLineTrace();
	}

	UpdateMovementSpeed();

	if (CableActor && bIsGrabbed)
	{
		FVector ControllerLocation = RightController->GetComponentLocation();
		FRotator ControllerRotation = RightController->GetComponentRotation();
		UE_LOG( LogTemp , Warning , TEXT( "RightController Location: %s, Rotation: %s" ) , *ControllerLocation.ToString() , *ControllerRotation.ToString() );
	}
}

// Called to bind functionality to input
void AHidePlayer::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	auto InputSystem = CastChecked<UEnhancedInputComponent>( PlayerInputComponent );
	if (InputSystem)
	{
		InputSystem->BindAction( IA_Move , ETriggerEvent::Triggered , this , &AHidePlayer::Move );
		InputSystem->BindAction( IA_Look , ETriggerEvent::Triggered , this , &AHidePlayer::Look );
		InputSystem->BindAction( IA_Grab , ETriggerEvent::Started , this , &AHidePlayer::OnActionTryGrab );
		InputSystem->BindAction( IA_Grab , ETriggerEvent::Completed , this , &AHidePlayer::OnActionUnGrab );
		InputSystem->BindAction( IA_Trigger , ETriggerEvent::Started , this , &AHidePlayer::OnActionTrigger );
		InputSystem->BindAction( IA_Trigger , ETriggerEvent::Completed , this , &AHidePlayer::OnActionUnTrigger );
		InputSystem->BindAction( IA_Run , ETriggerEvent::Started , this , &AHidePlayer::ONIARun );
		InputSystem->BindAction( IA_Run , ETriggerEvent::Completed , this , &AHidePlayer::ONIAUnRun );
		InputSystem->BindAction( IA_Crouch , ETriggerEvent::Started , this , &AHidePlayer::OnIACrouch );
		InputSystem->BindAction( IA_Crouch , ETriggerEvent::Completed , this , &AHidePlayer::OnIAUnCrouch );
	}
}

void AHidePlayer::OnConstruction( const FTransform& Transform )
{
	Super::OnConstruction( Transform );
}

void AHidePlayer::Move( const FInputActionValue& Value )
{
	// UE_LOG( LogTemp , Warning , TEXT( "Try" ) );

	FVector2D MovementVector = Value.Get<FVector2D>();

	// VR 플랫폼에서는 카메라의 방향을 기준으로 이동하므로, 카메라의 방향을 가져오기
	FVector ForwardVector = CameraComponent->GetForwardVector();
	FVector RightVector = CameraComponent->GetRightVector();

	// X 축은 카메라의 Forward 방향으로, Y 축은 카메라의 Right 방향으로 설정
	FVector MovementDirection = ForwardVector * MovementVector.Y + RightVector * MovementVector.X;

	// 이동 입력을 추가
	AddMovementInput( MovementDirection , 25.0f );

	//PC 일때 사용
	/*AddMovementInput( GetActorForwardVector() , MovementVector.X );
	AddMovementInput( GetActorRightVector() , MovementVector.Y );*/

	//VR 일때 카메라 기준으로 해야함
	//예) GetComponentForward??
	/*AddMovementInput(CameraComponent->GetForwardVector(), MovementVector.X);
	AddMovementInput(CameraComponent->GetRightVector(), MovementVector.Y);*/
}

void AHidePlayer::Look( const FInputActionValue& Value )
{

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		/*CameraComponent->bUsePawnControlRotation = true;*/

		AddControllerYawInput( LookAxisVector.X );
		AddControllerPitchInput( -LookAxisVector.Y );
	}

	//UE_LOG( LogTemp , Warning , TEXT( "Thumbstick Look: %s" ) , *LookAxisVector.ToString() );
}

void AHidePlayer::OnIACrouch( const FInputActionValue& Value )
{
	GetCharacterMovement()->Crouch();
	isCrouched = true;
	UE_LOG( LogTemp , Warning , TEXT( "Crouch Action Triggered: %s" ) , Value.Get<bool>() ? TEXT( "True" ) : TEXT( "False" ) );
}

void AHidePlayer::OnIAUnCrouch( const FInputActionValue& Value )
{
	GetCharacterMovement()->UnCrouch();
	isCrouched = false;
}


void AHidePlayer::ONIARun( const FInputActionValue& Value )
{
	UE_LOG( LogTemp , Warning , TEXT( "Crouch Action Triggered: %s" ) , Value.Get<bool>() ? TEXT( "True" ) : TEXT( "False" ) );
	bIsRun = true;
}

void AHidePlayer::ONIAUnRun( const FInputActionValue& Value )
{
	bIsRun = false;
}

void AHidePlayer::UpdateMovementSpeed()
{
	if (bIsRun)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300;
	}
}

UPrimitiveComponent* AHidePlayer::GetGrabbedObject() const
{
	return GrabbedObject;
}


bool AHidePlayer::IsTrigger() const
{
	return bIsTrigger;
}

bool AHidePlayer::IsGrab() const
{
	return bIsGrabbed;
}

void AHidePlayer::OnTriggerInteract( AInteraction* InteractionActor )
{
	InteractionActor->OnTriggerInteract( this );
}

void AHidePlayer::OnGrabInteract( AInteraction* InteractionActor )
{
	InteractionActor->OnGrabInteract( this );
}

const float MinGrabDistance = 15.0f;
void AHidePlayer::CheckOverlapWithNewEndStaticMesh()
{
	UE_LOG( LogTemp , Warning , TEXT( "CheckOverlapWithNewEndStaticMesh called. bIsGrabbed: %s, CableActor: %s" ) , bIsGrabbed ? TEXT( "true" ) : TEXT( "false" ) , CableActor ? *CableActor->GetName() : TEXT( "nullptr" ) );
	if (!bIsSecondGrabAttempted || !CableActor ) return;

	// 가장 가까운 NewEnd 컴포넌트와 그 거리를 저장할 변수들
	UPrimitiveComponent* ClosestNewEndComponent = nullptr;
	float MinDistance = TNumericLimits<float>::Max();

	// CableActor의 NewEnd 컴포넌트들을 확인하고 가장 가까운 것을 찾음
	TArray<UPrimitiveComponent*> NewEndComponents = { CableActor->NewEndStaticMesh, CableActor->NewEndMesh1, CableActor->NewEndMesh2 };
		for (UPrimitiveComponent* Component : NewEndComponents)
		{
			if (Component)
			{
				float Distance = FVector::Dist( Component->GetComponentLocation() , RightController->GetComponentLocation() );
				if (Distance < MinDistance && Distance <= MinGrabDistance)  // 거리가 최소 거리 조건보다 작거나 같을 때
				{
					MinDistance = Distance;
					ClosestNewEndComponent = Component;
				}
			}
		}

	// 가장 가까운 NewEndStaticMesh에 MoveMesh 연결
	if (ClosestNewEndComponent)
	{
		CableActor->HandleCableReleased( ClosestNewEndComponent ); // 여기서 HandleCableReleased 함수는 인자로 받은 NewEnd 컴포넌트를 처리하도록 수정되어야 함
		UE_LOG( LogTemp , Warning , TEXT( "Cable end attached to the closest new end mesh." ) );
		bIsGrabbed = false;
		bIsSecondGrabAttempted = false;
	}
}

void AHidePlayer::AttemptToGrabCable( const TArray<FOverlapResult>& OverlapResults )
{
	// CableActor와 MoveMesh의 유효성 검사
	float MinDistanceSquared = TNumericLimits<float>::Max();
	UPrimitiveComponent* ClosestComponent = nullptr;

	for (int i = 0; i < OverlapResults.Num(); ++i)
	{
		AActor* HitActor = OverlapResults[i].GetActor();
		if (HitActor && HitActor->IsA( ACable::StaticClass() )) {
			UPrimitiveComponent* Component = OverlapResults[i].GetComponent();
			float DistanceSquared = (RightController->GetComponentLocation() - Component->GetComponentLocation()).SizeSquared();
			if (DistanceSquared < MinDistanceSquared) {
				MinDistanceSquared = DistanceSquared;
				ClosestComponent = Component;
			}
		}
	}
	// 가장 가까운 컴포넌트가 있을 경우의 처리
	if (ClosestComponent)
	{
		FVector OverlapSphereCenter = RightController->GetComponentLocation();
		FVector ForwardVector = RightController->GetForwardVector();
		float DistanceFromController = 0.0f; // 예: 10cm 앞쪽

		FVector MoveMeshLocation = OverlapSphereCenter + (ForwardVector * DistanceFromController);
		FRotator MoveMeshRotation = RightController->GetComponentRotation();

		ClosestComponent->SetWorldLocationAndRotation( MoveMeshLocation , MoveMeshRotation );

		// 해당 CableActor에 대한 처리
		ACable* OverlappedCable = Cast<ACable>( ClosestComponent->GetOwner() );
		if (OverlappedCable)
		{
			OverlappedCable->HandleCableGrabbed( RightController , ClosestComponent );
			UE_LOG( LogTemp , Warning , TEXT( "Cable MoveMesh attached to RightController." ) );

			CableActor = OverlappedCable; // CableActor 할당
			bIsGrabbed = true;
			bIsSecondGrabAttempted = false;
			// 함수 종료 
			return;
		}
	}
}

void AHidePlayer::OnActionTryGrab()
{
	UE_LOG( LogTemp , Warning , TEXT( "Grab" ) );

	// RightController Null 체크
	if (!RightController)
	{
		UE_LOG( LogTemp , Warning , TEXT( "RightController is null." ) );
		return; // RightController가 null이면 여기서 함수 실행을 중단
	}

	FVector OverlapSphereCenter = RightController->GetComponentLocation();

	//Overlap Sphere와 겹친 물건을 저장할 Array
	TArray<FOverlapResult> HitObjects;
	FCollisionQueryParams CollisionParams;
	//무시할 Actor
	CollisionParams.AddIgnoredActor( this );
	CollisionParams.AddIgnoredComponent( RightController );
	//Overlap 실행
	bool bIsHit = GetWorld()->OverlapMultiByChannel( HitObjects , OverlapSphereCenter , FQuat::Identity , ECC_Visibility , FCollisionShape::MakeSphere( GrabRange ) );

	//Grab bool 깃발 초기화	
	//bIsGrabbed = false;

	//Overlap 결과가 없다면 밑에 코드 접근 불가
	if (!bIsHit)
	{
		UE_LOG( LogTemp , Warning , TEXT( "No object found to grab" ) );
		return;
	}

	AInteraction* ClosestInteractable = nullptr;
	float MinDistanceSquared = FLT_MAX; // 가능한 가장 큰 값을 초기 거리로 설정

	for (int i = 0; i < HitObjects.Num(); ++i) {
		AActor* HitActor = HitObjects[i].GetActor();
		if (HitActor && HitActor->IsA( AInteraction::StaticClass() )) {
			float DistanceSquared = (OverlapSphereCenter - HitObjects[i].GetComponent()->GetComponentLocation()).SizeSquared();
			if (DistanceSquared < MinDistanceSquared) {
				MinDistanceSquared = DistanceSquared;
				ClosestInteractable = Cast<AInteraction>( HitActor );
				// GrabbedObject를 현재 가장 가까운 Interactable의 컴포넌트로 설정
				GrabbedObject = HitObjects[i].GetComponent();
			}
		}
	}

	if (ClosestInteractable && GrabbedObject) 
	{
		// 여기서 GrabbedObject와 관련된 추가 작업 수행
		bIsGrabbed = true;
		GrabbedObject->SetSimulatePhysics( false );
		GrabbedObject->SetCollisionEnabled( ECollisionEnabled::NoCollision );
		GrabbedObject->AttachToComponent( RightController , FAttachmentTransformRules::KeepWorldTransform );
		OnTriggerInteract( ClosestInteractable );
	}
	//여기까진 잘 수행됨

	// 첫 번째 그랩 동작 처리
	if (!bIsGrabbed)
	{
		AttemptToGrabCable( HitObjects ); // 여기서 케이블을 잡는 로직 수행, 케이블을 잡으면 bIsGrabbed를 true로 설정
	}

	if (bIsGrabbed && !bIsSecondGrabAttempted)
	{
		UE_LOG( LogTemp , Warning , TEXT( "Second grab attempt detected." ) );
		bIsSecondGrabAttempted = true;
		CheckOverlapWithNewEndStaticMesh();
	}
}


void AHidePlayer::OnActionUnGrab()
{
	//이미 Grab 깃발이 내려가있다면 밑에 코드 접근 불가
	if (!bIsGrabbed) { return; }

	if (bIsGrabbed && GrabbedObject)
	{
		AInteraction* InteractionActor = Cast<AInteraction>( GrabbedObject->GetOwner() );
		if (InteractionActor)
		{
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
			Angle *= 0.1f;
			float ReducedTorquePower = TorquePower * 1.0f;
			FVector AngularVelocity = Axis * FMath::DegreesToRadians( Angle ) / GetWorld()->DeltaTimeSeconds;
			GrabbedObject->SetPhysicsAngularVelocityInRadians( AngularVelocity * ReducedTorquePower , true );
		}
	}
	
	//Grab한 물건을 놓았기 때문에 변수에 nullptr 할당 	
	bIsGrabbed = false;
	GrabbedObject = nullptr;
	CableActor = nullptr;
}

void AHidePlayer::RightGrabbing()
{
	//Grab bool 깃발이 올라가지 않았다면 밑에 코드 접근 불가
	if (!bIsGrabbed) { return; }

	//던지기 방향 저장
	ThrowDirection = RightController->GetComponentLocation() - PreviousGrabPosition;

	//잡기 위치 저장
	PreviousGrabRotation = RightController->GetComponentRotation().Quaternion();

	//위에 공식 참고
	//회전값 계산하기
	DeltaRotation = RightController->GetComponentQuat() * PreviousGrabRotation.Inverse();
	//회전값 저장하기
	PreviousGrabRotation = RightController->GetComponentQuat();

}

void AHidePlayer::PerformLineTrace()
{
	if (RightController && bIsTrigger)
	{
		FVector Start = RightController->GetComponentLocation(); // HandMesh 또는 HandController의 위치
		FVector ForwardVector = RightController->GetForwardVector(); // HandMesh 또는 HandController의 전방 벡터
		FVector DownVector = -RightController->GetUpVector(); // 컨트롤러의 위 방향의 반대
		// ForwardVector와 DownVector를 조합하여 라인트레이스의 방향을 아래로
		// 여기서는 ForwardVector의 90%와 DownVector의 10%를 조합
		FVector Direction = ForwardVector * 0.8f + DownVector * 0.2f;
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
				bHasInteracted = true;
			}
		}
		else
		{
			DrawDebugLine( GetWorld() , Start , End , FColor::Red , false , 0 , 0 , 0.1 );
		}

	}
}

void AHidePlayer::OnOverlapBegin( UPrimitiveComponent* OverlappedComp , AActor* OtherActor ,
								 UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if (OtherActor != nullptr && OtherActor != this && OtherActor->IsA( AVREnemyPlayer::StaticClass() ))
	{
		UE_LOG( LogTemp , Warning , TEXT( "Overlapped!!" ) );
		// count가 0 이하라면 함수 종료 
		if (LifeCount <= 0)
		{
			OnLifeDepleted();
			return;
		}
		// 생명 카운트 다운 
		LifeCount--;
		if (playerUI)
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

void AHidePlayer::OnOverlapEnd( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp ,
	int32 OtherBodyIndex )
{
	bLifeRemove = false;
}

void AHidePlayer::IncreaseLife()
{
	if (LifeCount < maxLifeCount && LifeCount >= 0)
	{
		LifeCount++;
		if (playerUI)
		{
			playerUI->AddLife();
		}
	}
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

bool AHidePlayer::LifeRemove()
{
	return bLifeRemove;
}

void AHidePlayer::UpdateTriggerStatus( bool bPressed )
{
	// 트리거 버튼 상태 업데이트
	bIsTrigger = bPressed;
	if (!bPressed)
	{
		// 버튼이 눌리지 않았을 때는 상호작용 상태를 리셋
		bHasInteracted = false;
	}
}

void AHidePlayer::HiddenPlayer()
{
	// 로컬 플레이어에게만 적용됨 
	BecomeInvisibleToLocalPlayer();

	// 이후 로직에서 서버에게 이 상태 변경을 알리기
	Server_SetPlayerHidden( true );

}

void AHidePlayer::Server_SetPlayerHidden_Implementation(bool bNewHidden)
{
	if (HasAuthority()) // 서버에서 실행 중인지 확인
	{
		bIsHidden = bNewHidden; // 상태를 변경

		if (bIsHidden)
		{
			BecomeInvisibleToOtherPlayers(); // 다른 클라이언트에게 플레이어를 '투명'으로 만들기

			// 5초 후에 상태를 복원하기 위한 타이머 설정
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer( TimerHandle , this , &AHidePlayer::RestoreVisibility , 5.0f , false );
		}
	}
}

bool AHidePlayer::Server_SetPlayerHidden_Validate(bool bNewHidden)
{
	return true;
}

void AHidePlayer::SetPlayerHidden()
{
	if (IsLocallyControlled())
	{
		BecomeInvisibleToLocalPlayer(); // 로컬 변경 
		Server_SetPlayerHidden( true ); // 서버에 상태 변경을 요청
	}
}

void AHidePlayer::RestoreVisibility()
{
	if (IsLocallyControlled())
	{
		BecomeVisibleToLocalPlayer(); // 로컬 변경
		Server_SetPlayerHidden( false ); // 서버에 상태 변경을 요청
	}
}

void AHidePlayer::OnRep_IsHidden()
{
	if (bIsHidden)
	{
		BecomeInvisibleToOtherPlayers();
	}
	else
	{
		BecomeVisibleToOtherPlayers();
	}
}
void AHidePlayer::BecomeInvisibleToOtherPlayers()
{
	// 액터 전체를 숨김 
	SetActorHiddenInGame( true );

	// 콜리전 비활성화 
	SetActorEnableCollision( false );
}

void AHidePlayer::BecomeVisibleToOtherPlayers()
{
	// 액터 전체를 다시 보이게하기
	SetActorHiddenInGame( false );

	// 콜리전 다시 활성화 
	SetActorEnableCollision( true );
}

void AHidePlayer::BecomeInvisibleToLocalPlayer()
{
	// 플레이어 메시에 접근
	UMeshComponent* PlayerMesh = GetMesh();

	// 새로운 투명화 머티리얼 찾기
	UMaterialInterface* InvisibleMaterial1 = LoadObject<UMaterialInterface>( nullptr , TEXT( "MaterialInstanceConstant'/Game/JH/Material/MI_MannequinGlow05.MI_MannequinGlow05'" ) );
	UMaterialInterface* InvisibleMaterial2 = LoadObject<UMaterialInterface>( nullptr , TEXT( "MaterialInstanceConstant'/Game/JH/Material/MI_MannequinGlow06.MI_MannequinGlow06'" ) );

	// 메시의 모든 섹션에 대해 머티리얼 변경
	PlayerMesh->SetMaterial( 0 , InvisibleMaterial1 );
	PlayerMesh->SetMaterial( 1 , InvisibleMaterial2 );

	BecomeInvisibleToOtherPlayers();
}

void AHidePlayer::BecomeVisibleToLocalPlayer()
{
	// 메시의 모든 섹션에 대해 원래의 머티리얼로 복원
	UMeshComponent* PlayerMesh = GetMesh();
	PlayerMesh->SetMaterial( 0 , OriginalMaterial1 );
	PlayerMesh->SetMaterial( 1 , OriginalMaterial2 );

	BecomeVisibleToOtherPlayers();
}

void AHidePlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( AHidePlayer , bIsHidden );
}

void AHidePlayer::OnActionTrigger()
{
	UE_LOG( LogTemp , Warning , TEXT( "Trigger" ) );

	bIsTrigger = true;
	/*bIsTriggerPressed =true;*/
	bHasInteracted = false; // 트리거를 다시 누를 때마다 상호작용을 리셋
}

void AHidePlayer::OnActionUnTrigger()
{
	UE_LOG( LogTemp , Warning , TEXT( "UnTrigger" ) );

	bIsTrigger = false;
	//bIsTriggerPressed = false;
}

