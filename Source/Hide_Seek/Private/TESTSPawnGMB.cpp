// Fill out your copyright notice in the Description page of Project Settings.


#include "TESTSPawnGMB.h"
#include "HidePlayer.h"
#include "VREnemyPlayer.h"
#include "Net/UnrealNetwork.h"

ATESTSPawnGMB::ATESTSPawnGMB()
{
	
}


UClass* ATESTSPawnGMB::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (currentPawnToAssign)
	{
		if (firstPawn != nullptr && secondPawn != nullptr)
		{
			if(currentPawnToAssign == firstPawn)
			{
				currentPawnToAssign = secondPawn;
			}
			else
			{
				currentPawnToAssign = firstPawn;
			}
		}
	}
	else
	{
		if (firstPawn != nullptr && secondPawn != nullptr)
		{
			return currentPawnToAssign = ((rand() % 1) == 0) ? firstPawn : secondPawn;
		}
	}

	return currentPawnToAssign;
}

void ATESTSPawnGMB::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME( ATESTSPawnGMB , currentPawnToAssign );
}
