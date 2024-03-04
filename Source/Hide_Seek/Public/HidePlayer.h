// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "HidePlayer.generated.h"

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

	UPROPERTY(EditAnywhere)
	float Movespeed = 600;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* IMC_JHVRInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Move;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Look;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Grab;
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Trigger;*/

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* LeftControllerCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* RightControllerCollision;

private:
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

public:

	//Grab Overlap 할때 사용하는 Grab 범위 또는 Overlap Sphere 크기
	UPROPERTY(EditAnywhere, Category = "Grab", meta = (AllowPrivateAccess = true))
	float GrabRange = 100;

	//Grab Input 버튼 누를때 실행됨
	UFUNCTION()
	void OnActionTryGrab();

	//Grab Input 버튼 땔때 실행됨 
	UFUNCTION()
	void OnActionUnGrab();

	//Tick에서 실행됨
	//만약에 bIsGrabbed이 True면 프레임마다 호출됨
	void RightGrabbing();

	//Input 버튼이 눌렀는지/땠는지 알려주는 bool 변수
	//이걸로 누름/땜 상태를 제어할 수 있음
	bool bIsGrabbed;

	// bIsTriggered가 True이면 프레임마다 호출됨 
	/*void TriggerGragging();
	bool bIsTriggered;*/

	////Trigger Input 버튼 누를때 실행됨
	//UFUNCTION()
	//void OnActionTrigger();

	////Trigger Input 버튼 땔때 실행됨 
	//UFUNCTION()
	//void OnActionUnTrigger();

	// 상호작용 인터페이스
	void ProcessInteraction(AActor* Actor);

	//Grab된 물건의 Component
	UPROPERTY()
	class UPrimitiveComponent* GrabbedObject;

	//던지는 방향
	FVector ThrowDirection;

	//던질때 힘
	UPROPERTY(EditAnywhere, Category = "Grab", meta = (AllowPrivateAccess = true))
	float ThrowStrength = 10000;

	//Grab위치를 실시간으로 업데이트 해야하기 때문에 여기다가 저장함
	FVector PreviousGrabPosition;
	//실시간으로 저장할 회전값
	FQuat PreviousGrabRotation;
	//회전 변화량
	FQuat DeltaRotation;

	//회전 힘
	UPROPERTY(EditAnywhere, Category = "Grab", meta = (AllowPrivateAccess = true))
	float TorquePower = 1;

};
