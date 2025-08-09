// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Game/ShadowGameMode.h"

#include "Kismet/GameplayStatics.h"





void AShadowGameMode::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

	FHitResult HitResult;
	TraceUnderCrosshair(HitResult);
}

void AShadowGameMode::TraceUnderCrosshair(FHitResult& HitResult)
{
	FVector2D ViewSize;
	if (GEngine && GEngine->GameViewport)
	{
		/** Get size of the vieport */
		GEngine->GameViewport->GetViewportSize(ViewSize);
	}

	/** Make Crosshair location in the midle of the screen */
	FVector2D CrosshairLocation(ViewSize.X / 2, ViewSize.Y / 2);

	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		/** For LineTrace: Start position is on center of screen, End is 80.000 unit outworld from start position */
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + (CrosshairWorldDirection * 80000.0f);

		/** It will trace strait out and it will hit first blocking hit */
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);

		/** If not get hit then HitResult = End */
		if (!HitResult.bBlockingHit)
		{
			HitResult.ImpactPoint = End;
			HitTarget = End;
		}
		else
		{
			HitTarget = HitResult.ImpactPoint;
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 12.0f, 12, FColor::Red);
		}
	}
}
