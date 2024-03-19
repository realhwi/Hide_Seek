// All Rights Reserved - 2024 - ReEngage Developments Ltd

#include "Multi_WidgetComponent.h"

void UMulti_WidgetComponent::copyProperties(const UWidgetComponent* copyFrom)
{
	SetDrawSize(copyFrom->GetDrawSize());
	SetDrawAtDesiredSize(copyFrom->GetDrawAtDesiredSize());
	SetGeometryMode(copyFrom->GetGeometryMode());
	SetCylinderArcAngle(copyFrom->GetCylinderArcAngle());
	SetBlendMode(copyFrom->GetBlendMode());
	SetTwoSided(copyFrom->GetTwoSided());
	SetTickWhenOffscreen(copyFrom->GetTickWhenOffscreen());
	SetMaterial(0, copyFrom->GetMaterial(0));
}

void UMulti_WidgetComponent::setCustomChannel(ECollisionChannel newCustomChannel)
{
	customChannel = newCustomChannel;
	bUseCustomChannel = true;

	// Update current collision
	if (GetWidget())
		OnWidgetVisibilityChanged(GetWidget()->GetVisibility());
}

void UMulti_WidgetComponent::setDontUseCustomChannel()
{
	bUseCustomChannel = false;

	// Update current collision
	if (GetWidget())
		OnWidgetVisibilityChanged(GetWidget()->GetVisibility());
}

void UMulti_WidgetComponent::SetWidget(UUserWidget* InWidget)
{
	Super::SetWidget(InWidget);

	if (InWidget)
	{
		// Establish visibility changed delegate
		InWidget->OnVisibilityChanged.AddDynamic(this, &UMulti_WidgetComponent::OnWidgetVisibilityChanged);

		// Update to widget's current visibility
		OnWidgetVisibilityChanged(InWidget->GetVisibility());
	}
}

void UMulti_WidgetComponent::OnWidgetVisibilityChanged(ESlateVisibility InVisibility)
{
	switch (InVisibility)
	{
	// If widget has become visible, make this widget component have collision
	case ESlateVisibility::Visible:
	case ESlateVisibility::SelfHitTestInvisible:
	case ESlateVisibility::HitTestInvisible:
	{
		SetCollisionProfileName(TEXT("UI"));

		// If using custom channel
		if (bUseCustomChannel)
			SetCollisionResponseToChannel(customChannel, ECollisionResponse::ECR_Overlap);

		break;
	}
	// Otherwise, all other forms of visibility require this widget component to not have collision
	default:
	{
		SetCollisionProfileName(TEXT("NoCollision"));

		// If using custom channel
		if (bUseCustomChannel)
			SetCollisionResponseToChannel(customChannel, ECollisionResponse::ECR_Ignore);
	}
	}
}