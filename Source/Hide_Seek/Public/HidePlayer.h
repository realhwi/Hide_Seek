// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRCharacter.h"
#include "Logging/LogMacros.h"
#include "HidePlayer.generated.h"


class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class HIDE_SEEK_API AHidePlayer : public AVRCharacter
{
	GENERATED_BODY()

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	virtual void BeginPlay();

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

public:
	AHidePlayer();
};
