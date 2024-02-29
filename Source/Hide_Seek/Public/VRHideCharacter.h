// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "VRHideCharacter.generated.h"

UCLASS()
class HIDE_SEEK_API AVRHideCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Sets default values for this character's properties
	AVRHideCharacter();

	UPROPERTY( EditAnywhere )
	float Movespeed = 600;

	UPROPERTY( EditAnywhere, BlueprintReadOnly , Category = Input , meta = (AllowPrivateAccess = "true") )
	class UInputMappingContext* IMC_JHVRInput;

	UPROPERTY( EditAnywhere, BlueprintReadOnly , Category = Input , meta = (AllowPrivateAccess = "true") )
	class UInputAction* IA_Move;
	UPROPERTY( EditAnywhere, BlueprintReadOnly , Category = Input , meta = (AllowPrivateAccess = "true") )
	class UInputAction* IA_Look;

	UPROPERTY( EditAnywhere )
	class UCameraComponent* CameraComponent;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Player Settings | Player")
	class UMotionControllerComponent* LeftController;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Player Settings | Player")
	class UMotionControllerComponent* RightController;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Player Settings | Player")
	class USkeletalMeshComponent* LeftHandMesh;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = "Player Settings | Player")
	class USkeletalMeshComponent* RightHandMesh;


private:
	UFUNCTION()
	void Move( const FInputActionValue& Value );

	UFUNCTION()
	void Look( const FInputActionValue& Value );


};
