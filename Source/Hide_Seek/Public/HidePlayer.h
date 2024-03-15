// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Interaction.h"
#include "Components/WidgetComponent.h"
#include "HidePlayer.generated.h"

class ACable; // 전방 선언을 추가합니다.
UCLASS()
class HIDE_SEEK_API AHidePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHidePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	//UPROPERTY(EditAnywhere)
	//float Movespeed = 300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* IMC_JHVRInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Move;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Look;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Grab;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Trigger;
	UPROPERTY( EditAnywhere , BlueprintReadOnly , Category = Input , meta = (AllowPrivateAccess = "true") )
	class UInputAction* IA_Run;
	UPROPERTY( EditAnywhere , BlueprintReadOnly , Category = Input , meta = (AllowPrivateAccess = "true") )
	class UInputAction* IA_Crouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings | Player")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings | Player")
	class UMotionControllerComponent* LeftController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings | Player")
	class UMotionControllerComponent* RightController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings | Player")
	class UStaticMeshComponent* LeftHandMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings | Player")
	class UStaticMeshComponent* RightHandMesh;


private:
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UPROPERTY( Transient )
	UPrimitiveComponent* GrabbedComponent; // 현재 잡고 있는 컴포넌트

public:

	// 상호작용 액터
	// Trigger와 Grab 상태 확인
	UFUNCTION( BlueprintCallable , Category = "Interaction" )
	bool IsTrigger() const;

	UFUNCTION( BlueprintCallable , Category = "Interaction" )
	bool IsGrab() const;

	void OnTriggerInteract( AInteraction* InteractionActor );
	void OnGrabInteract( AInteraction* InteractionActor );

	//Grab Overlap 할때 사용하는 Grab 범위 또는 Overlap Sphere 크기
	UPROPERTY(EditAnywhere, Category = "Grab", meta = (AllowPrivateAccess = true))
	float GrabRange = 50;

	//Grab Input 버튼 누를때 실행됨
	UFUNCTION( BlueprintCallable, Category = "Interaction" )
	void OnActionTryGrab();

	//Grab Input 버튼 땔때 실행됨 
	UFUNCTION( BlueprintCallable, Category = "Interaction" )
	void OnActionUnGrab();

	//Tick에서 실행됨
	//만약에 bIsGrabbed이 True면 프레임마다 호출됨
	UFUNCTION()
	void RightGrabbing();
	//이걸로 누름/땜 상태를 제어할 수 있음
	bool bIsGrabbed = false;

	//Trigger Input 버튼 누를때 실행됨
	UFUNCTION(BlueprintCallable)
	void OnActionTrigger();
	// bIsTriggered가 True이면 프레임마다 호출됨
	bool bIsTrigger=false;

	//Trigger Input 버튼 땔때 실행됨 
	UFUNCTION()
	void OnActionUnTrigger();

	// LineTracer 기능 구현
	UFUNCTION()
	void PerformLineTrace();

	UFUNCTION()
	void OnIACrouch( const FInputActionValue& Value );

	UFUNCTION()
	void OnIAUnCrouch( const FInputActionValue& Value );

	// 쑤그리기 
	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	bool isCrouched;

	UFUNCTION()
	void ONIARun( const FInputActionValue& Value );

	UFUNCTION()
	void ONIAUnRun( const FInputActionValue& Value );

	UFUNCTION()
	void UpdateMovementSpeed();

	bool bIsRun = false;

public:

	/*인터렉션 관련 기능들*/
	//Grab된 물건의 Component
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	class UPrimitiveComponent* GrabbedObject;

	UFUNCTION()
	UPrimitiveComponent* GetGrabbedObject() const;

	//던지는 방향
	FVector ThrowDirection;
	FVector PreviousGrabLocation;

	//던질때 힘
	UPROPERTY(EditAnywhere, Category = "Grab", meta = (AllowPrivateAccess = true))
	float ThrowStrength;

	//Grab위치를 실시간으로 업데이트 해야하기 때문에 여기다가 저장함
	FVector PreviousGrabPosition;
	//실시간으로 저장할 회전값
	FQuat PreviousGrabRotation;
	//회전 변화량
	FQuat DeltaRotation;
	

	//회전 힘
	UPROPERTY(EditAnywhere, Category = "Grab", meta = (AllowPrivateAccess = true))
	float TorquePower = 10;

	// Player Enemy Overlap GameOver Event
	UFUNCTION()
	void OnOverlapBegin( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult );
	UFUNCTION()
	virtual void OnOverlapEnd( UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex );


	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite )
	class UPlayerUI* playerUI;

	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite )
	TSubclassOf<class UPlayerUI> playerUIFactory;

	UPROPERTY( VisibleAnywhere , BlueprintReadOnly , Category = "Components" )
	UWidgetComponent* playerWidgetComponent;

	UPROPERTY( EditDefaultsOnly )
	class UGameOver* GameOverUI;

	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite )
	TSubclassOf<class UGameOver> GameOverUIFactory;

	UPROPERTY( EditDefaultsOnly )
	// 생명 최대 값 
	int32 maxLifeCount = 3;
	// 1부터 시작해서 max까지
	int32 LifeCount = maxLifeCount;

	// 충돌 확인 
	UPROPERTY( EditAnywhere )
	class UParticleSystem* VFX;

	// Die 
	UFUNCTION()
	void OnLifeDepleted();

	// 생명 하나 감소, 이게 true가되면 텔레포트 하기
	UPROPERTY( EditDefaultsOnly )
	bool bLifeRemove = false;

	// 인터렉션 실행 감지
	UPROPERTY(EditAnywhere)
	bool bHasInteracted = false;

	void UpdateTriggerStatus( bool bPressed );

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	ACable* CableActor;

};
