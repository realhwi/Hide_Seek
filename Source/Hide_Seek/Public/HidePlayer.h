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
	class UInputAction* IA_RightGrab;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Trigger;

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

	//Grab Overlap �Ҷ� ����ϴ� Grab ���� �Ǵ� Overlap Sphere ũ��
	UPROPERTY(EditAnywhere, Category = "Grab", meta = (AllowPrivateAccess = true))
	float GrabRange = 100;

	//Grab Input ��ư ������ �����
	UFUNCTION()
	void OnActionTryRightGrab();

	//Grab Input ��ư ���� ����� 
	UFUNCTION()
	void OnActionUnRightGrab();

	//Tick���� �����
	//���࿡ bIsGrabbed�� True�� �����Ӹ��� ȣ���
	void RightGrabbing();

	//Input ��ư�� ��������/������ �˷��ִ� bool ����
	//�̰ɷ� ����/�� ���¸� ������ �� ����
	bool bIsGrabbed;

	// bIsTriggered�� True�̸� �����Ӹ��� ȣ��� 
	void TriggerGragging();
	bool bIsTriggered;

	//Trigger Input ��ư ������ �����
	UFUNCTION()
	void OnActionTrigger();

	//Trigger Input ��ư ���� ����� 
	UFUNCTION()
	void OnActionUnTrigger();

	// ��ȣ�ۿ� �������̽�
	void ProcessInteraction(AActor* Actor);

	//Grab�� ������ Component
	UPROPERTY()
	class UPrimitiveComponent* GrabbedObject;

	//������ ����
	FVector ThrowDirection;

	//������ ��
	UPROPERTY(EditAnywhere, Category = "Grab", meta = (AllowPrivateAccess = true))
	float ThrowStrength = 10000;

	//Grab��ġ�� �ǽð����� ������Ʈ �ؾ��ϱ� ������ ����ٰ� ������
	FVector PreviousGrabPosition;
	//�ǽð����� ������ ȸ����
	FQuat PreviousGrabRotation;
	//ȸ�� ��ȭ��
	FQuat DeltaRotation;

	//ȸ�� ��
	UPROPERTY(EditAnywhere, Category = "Grab", meta = (AllowPrivateAccess = true))
	float TorquePower = 1;

};
