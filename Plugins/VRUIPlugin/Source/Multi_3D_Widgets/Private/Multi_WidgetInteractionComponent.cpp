// All Rights Reserved - 2024 - ReEngage Developments Ltd

#include "Multi_WidgetInteractionComponent.h"

#include "UnrealEngine.h"

#include "Multi_UserWidget.h"
#include "Multi_WidgetComponent.h"

#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/SlateUser.h"

FKey UMulti_WidgetInteractionComponent::queryKey = FKey("Multi_Query_Key");

UMulti_WidgetInteractionComponent::UMulti_WidgetInteractionComponent()
{
	// Default camera collision is used becasue widget components are visible but do not block this trace. 
	TraceChannel = ECollisionChannel::ECC_Camera;
}

void UMulti_WidgetInteractionComponent::PressPointerKey(FKey Key)
{
	Super::PressPointerKey(Key);

#if !UE_BUILD_SHIPPING 

	auto logMsg = [&](const FString& msg)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, msg);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
		};

	if (bDebugInteractions)
	{
		const auto hitActor = GetLastHitResult().GetActor();
		if (!hitActor)
		{
			const auto msg = FString::Printf(TEXT("Multi_WidgetInteractionComponent: Didn't overlap anything to interact with."));
			logMsg(msg);
			return;
		}
		
		const auto component = Cast<UMulti_WidgetComponent>(GetLastHitResult().GetComponent());
		if (!component)
		{
			const auto msg = FString::Printf(TEXT("Multi_WidgetInteractionComponent: Didn't interact with a displayer. Interacted object: %s"), *hitActor->GetName());
			logMsg(msg);
			return;
		}

		const auto userwidget = component->GetWidget();
		if (!userwidget)
		{
			const auto msg = FString::Printf(TEXT("Multi_WidgetInteractionComponent: Displayer did not have a userwidget."));
			logMsg(msg);
			return;
		}

		const auto multi_userwidget = Cast<UMulti_UserWidget>(userwidget);
		if (!multi_userwidget)
		{
			const auto msg = FString::Printf(TEXT("Multi_WidgetInteractionComponent: Interacted with a UserWidget that doesn't inherit from Multi_UserWidget! This will not allow for Z-Order Interactions unless it inherits from Multi_UserWidget! UserWidget name: %s"), *userwidget->GetName());
			logMsg(msg);
			return;
		}

		const auto msg = FString::Printf(TEXT("Multi_WidgetInteractionComponent: Interacted with Multi_UserWidget: %s"), *multi_userwidget->GetName());
		logMsg(msg);
	}

#endif
}

UWidgetInteractionComponent::FWidgetTraceResult UMulti_WidgetInteractionComponent::PerformTrace() const
{
	/* ------------------------ Setup Input Device ID ------------------------ */

	// Find the primary input device for this Slate User
	FInputDeviceId InputDeviceId = INPUTDEVICEID_NONE;
	if (TSharedPtr<FSlateUser> SlateUser = FSlateApplication::Get().GetUser(VirtualUser->GetUserIndex()))
	{
		FPlatformUserId PlatUser = SlateUser->GetPlatformUserId();
		InputDeviceId = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(PlatUser);
	}

	// Just in case there was no input device assigned to this virtual user, get the default platform
	// input device
	if (!InputDeviceId.IsValid())
	{
		InputDeviceId = IPlatformInputDeviceMapper::Get().GetDefaultInputDevice();
	}

	FWidgetTraceResult TraceResult;

	auto performTrace = [&](ECollisionChannel traceChannel)
		{
			/* ------------------------ Perform Line Trace ------------------------ */
			TArray<FHitResult> MultiHits;

			const FVector WorldLocation = GetComponentLocation();
			const FTransform WorldTransform = GetComponentTransform();
			const auto WorldDirection = WorldTransform.GetUnitAxis(EAxis::X);

			TArray<UPrimitiveComponent*> PrimitiveChildren;
			GetRelatedComponentsToIgnoreInAutomaticHitTesting(PrimitiveChildren);

			FCollisionQueryParams Params(SCENE_QUERY_STAT(WidgetInteractionComponentTrace));
			Params.AddIgnoredComponents(PrimitiveChildren);

			TraceResult.LineStartLocation = WorldLocation;
			TraceResult.LineEndLocation = WorldLocation + (WorldDirection * InteractionDistance);

			GetWorld()->LineTraceMultiByChannel(MultiHits, TraceResult.LineStartLocation, TraceResult.LineEndLocation, traceChannel, Params);

			/* ------------------------ Search for the widget component (widget) to interact with ------------------------ */

			for (const FHitResult& HitResult : MultiHits)
			{
				if (UWidgetComponent* HitWidgetComponent = Cast<UWidgetComponent>(HitResult.GetComponent()))
				{
					// If this widget is visible
					if (HitWidgetComponent->IsVisible())
					{
						bool bInteractWithThisWidget = false;

						TraceResult.HitWidgetComponent = HitWidgetComponent;
						TraceResult.HitResult = HitResult;

						if (TraceResult.HitWidgetComponent->GetGeometryMode() == EWidgetGeometryMode::Cylinder)
						{
							TTuple<FVector, FVector2D> CylinderHitLocation = TraceResult.HitWidgetComponent->GetCylinderHitLocation(TraceResult.HitResult.ImpactPoint, WorldDirection);
							TraceResult.HitResult.ImpactPoint = CylinderHitLocation.Get<0>();
							TraceResult.LocalHitLocation = CylinderHitLocation.Get<1>();
						}
						else
						{
							ensure(TraceResult.HitWidgetComponent->GetGeometryMode() == EWidgetGeometryMode::Plane);
							TraceResult.HitWidgetComponent->GetLocalHitLocation(TraceResult.HitResult.ImpactPoint, TraceResult.LocalHitLocation);
						}
						TraceResult.HitWidgetPath = FindHoveredWidgetPath(TraceResult);

						// If this is a userwidget that supports z order
						auto multi_userwidget = Cast<UMulti_UserWidget>(HitWidgetComponent->GetWidget());
						if (multi_userwidget)
						{
							// Put the query key into the pointer event
							FPointerEvent PointerEvent = FPointerEvent(
								InputDeviceId,
								(uint32)PointerIndex,
								TraceResult.LocalHitLocation,
								TraceResult.LocalHitLocation,
								PressedKeys,
								queryKey,
								0.0f,
								ModifierKeys,
								VirtualUser->GetUserIndex());

							// Send the query key move event
							FSlateApplication::Get().RoutePointerMoveEvent(TraceResult.HitWidgetPath, PointerEvent, false);

							// True if widget was interacted with. Else, continue searching
							if (multi_userwidget->isQueryWidget())
							{
								bInteractWithThisWidget = true;
							}
						}
						// Otherwise, just interact with this user widget
						else
						{
							bInteractWithThisWidget = true;
						}

						// If this is the widget to interact with
						if (bInteractWithThisWidget)
						{
							TraceResult.bWasHit = true;
							break;
						}
					}
				}
				else if (HitResult.bBlockingHit)
				{
					// If we hit something that wasn't a widget component, we're done.

					break;
				}
			}
		};

	if (bUseCustomChannel)
	{
		// First use custom channel
		performTrace(customChannel);

		// If didn't hit anything and this interactor is specified 
		if (!TraceResult.bWasHit && bAlsoTestDefaultChannel)
			performTrace(TraceChannel);
	}
	else
	{
		// Just use default channel
		performTrace(TraceChannel);
	}

	return TraceResult;
}

