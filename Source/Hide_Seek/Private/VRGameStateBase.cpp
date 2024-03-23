// Fill out your copyright notice in the Description page of Project Settings.


#include "VRGameStateBase.h"

#include "VREnemyPlayer.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AVRGameStateBase::AVRGameStateBase()
{
	SetReplicates( true );
}

void AVRGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	countPlayTime = maxPlayTime;
}

void AVRGameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bFoundEnemy && !bGameReady)
	{
		FindEnemyPlayer();
	} 
	else if(bFoundEnemy && !bGameReady)
	{
		bGameReady = true;
		seconds = UGameplayStatics::GetRealTimeSeconds( GetWorld() );

	}
	else if(bGameReady && bFoundEnemy)
	{
		//Timer 시작
		CountTime();
	}

	UE_LOG( LogTemp , Warning , TEXT( "%s, %f" ) , *GetName(), countPlayTime );

}

void AVRGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME( AVRGameStateBase , countPlayTime );
}

void AVRGameStateBase::FindEnemyPlayer()
{
	
	for (int i = 0; i < PlayerArray.Num(); i++)
	{
		auto playerStates = Cast<APlayerState>( PlayerArray[i] );
		if (playerStates && playerStates->GetPlayerController() && playerStates->GetPlayerController()->GetPawn())
		{
			EnemyPlayer = Cast<AVREnemyPlayer>( playerStates->GetPlayerController()->GetPawn() );
			if (EnemyPlayer)
			{
				bFoundEnemy = true;
				break;
			}
		}
	}
}

void AVRGameStateBase::CountTime()
{
	if(countPlayTime <= 0)
	{
		countPlayTime = 0;
	}
	else
	{
		countPlayTime = (UGameplayStatics::GetRealTimeSeconds( GetWorld() ) - seconds);
		UE_LOG( LogTemp , Warning , TEXT( "%f" ) , countPlayTime );

		if (countPlayTime >= 300)
		{
			countPlayTime = 300;

			isTimeOut = true;
		}

		// countPlayTime -= GetWorld()->GetTimeSeconds() - seconds;
	}
}
