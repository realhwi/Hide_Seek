// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "VREnemyPlayer.generated.h"

UCLASS()
class HIDE_SEEK_API AVREnemyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVREnemyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float moveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* handSphereColl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputMappingContext* IMC_VREnemyInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* IA_VREnemyMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* IA_VREnemyLook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* IA_EnemyInteraction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* cameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMotionControllerComponent* leftMotionCtrl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMotionControllerComponent* rightMotionCtrl;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class USkeletalMeshComponent* leftHandMesh;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class USkeletalMeshComponent* rightHandMesh;

private:
	UFUNCTION()
	void EnemyMove(const FInputActionValue& value);

	UFUNCTION()
	void EnemyLook(const FInputActionValue& value);

	UFUNCTION()
	void EInteractionStart(const FInputActionValue& value);

	//UFUNCTION()
	//void EInteractionTriggered(const FInputActionValue& value);

	UFUNCTION()
	void EInteractionComplete(const FInputActionValue& value);

	bool bHasController = false;
	//UPROPERTY()
	//class UEnhancedInputComponent* InputComponents;

public:
	UFUNCTION()
	void FirstSkillActive();

	UFUNCTION()
	void SecondSkillActive();

	UFUNCTION()
	void ThirdSkillActive();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float activeSkillTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float activeMaxSkillTime = 180;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool activeFirstSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool activeSecondSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool activeThirdSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 countHPChip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool inAltar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AHidePlayer* playerChar;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	class AActor* door;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	class AActor* itemBox;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	class AActor* enemyAltar;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float findActorRadius = 150;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	float itemLockTime;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	float electDuctLockTime;

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	float escapeObjLockTime;

	UPROPERTY( EditAnywhere , BlueprintReadWrite, Replicated )
	bool isHandUP;

	UPROPERTY( EditAnywhere , BlueprintReadWrite, Replicated )
	bool canCheckActor;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ActionHandUp();

	UFUNCTION( Server , Reliable )
	void ServerRPC_ActionHandDown();

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
};