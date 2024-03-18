// All Rights Reserved - 2023 - ReEngage Developments Ltd

#pragma once

#include "CoreMinimal.h"
#include "Multi_Displayer.h"
#include "VR_UI_Displayer_Mobile.generated.h"

/**
 * This class is intended to function as a menu players can open and close whilst moving around in VR. Much like how you would open or close the options menu in non-VR, for example
 * 
 * You can set it to show or hide (setVisible()) and set it to follow the player or stay still (setFollowPlayer())
 * You can set the distance from player (float distanceFromPlayer)
 */
UCLASS()
class VR_UI_API AMulti_Displayer_Mobile : public AMulti_Displayer
{
	GENERATED_BODY()
public:
	AMulti_Displayer_Mobile();
	virtual void Tick(float DeltaTime) override;

	// Setters
	UFUNCTION(BlueprintCallable, Category = "VR UI Manager")
	void setVisible(bool bVisible);
	UFUNCTION(BlueprintCallable, Category = "VR UI Manager")
	void toggleVisibility();
	UFUNCTION(BlueprintCallable, Category = "VR UI Manager")
	void setFollowPlayer(bool bFollowPlayer_);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mobile VR UI Displayer", meta=(Units="cm"))
	float distanceFromPlayer = 300.f;

	class APlayerController* localPlayerController = 0;

	bool bDisplayerVisible = false;
	bool bFollowPlayer = true;
};
