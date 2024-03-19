// Fill out your copyright notice in the Description page of Project Settings.


#include "HideAnim.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HidePlayer.h"

void UHideAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	AHidePlayer* player = Cast<AHidePlayer>(TryGetPawnOwner());
	if(nullptr == player)
	{
		return;
	}
	FVector v= player->GetVelocity();
	FVector forward = player->GetActorForwardVector();
	FVector right = player->GetActorRightVector();

	speed = FVector::DotProduct(v,forward);
	direction = FVector::DotProduct(v,right);

	isInAir = player->GetCharacterMovement()->IsFalling();
	isCrouched = player->isCrouched;
	pitch = player->GetBaseAimRotation().Pitch;
}
