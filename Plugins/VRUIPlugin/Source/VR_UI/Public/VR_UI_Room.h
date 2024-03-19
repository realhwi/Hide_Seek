// All Rights Reserved - 2023 - ReEngage Developments Ltd

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VR_UI_Room.generated.h"

UCLASS()
class VR_UI_API AVR_UI_Room : public AActor
{
	GENERATED_BODY()
	
public:	
	AVR_UI_Room();
	virtual void PostInitializeComponents() override;

	// Getters
	UFUNCTION(BlueprintCallable, Category = "VR UI")
	class AMulti_Displayer* getDisplayer();
	UFUNCTION(BlueprintCallable, Category = "VR UI")
	AActor* getPlayerStart();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "VR UI")
	class UMulti_DisplayerComponent* displayerComponent = 0;
	UChildActorComponent* playerStartChildComponent = 0;
};
