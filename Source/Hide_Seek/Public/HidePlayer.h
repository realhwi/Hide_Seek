// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRCharacter.h"
#include "Logging/LogMacros.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "HidePlayer.generated.h"


class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class HIDE_SEEK_API AHidePlayer : public AVRCharacter
{
	GENERATED_BODY()

public:
	AHidePlayer();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;


public:

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	float Movespeed = 600;


	UPROPERTY(EditAnywhere)
	class UInputMappingContext* IMC_VRInput;

	UPROPERTY( EditAnywhere )
	class UInputAction* IA_Move;
	UPROPERTY( EditAnywhere )
	class UInputAction* IA_Look;

	UPROPERTY( EditAnywhere )
	class UCameraComponent* CameraComponent;


private:
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);


};
