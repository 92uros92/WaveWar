// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "Player/ShadowPlayerController.h"
#include "Interaction/EnemyInterface.h"





AShadowPlayerController::AShadowPlayerController()
{

}

void AShadowPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//CursorTrace();
}

void AShadowPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Crosshairs;

	//FInputModeGameAndUI InputModeData;
	//InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//InputModeData.SetHideCursorDuringCapture(false);
	//SetInputMode(InputModeData);
}

//void AShadowPlayerController::CursorTrace()
//{
//	FHitResult CursorHit;
//	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
//	
//
//	if (!CursorHit.bBlockingHit)
//		return;
//
//	LastActor = ThisActor;
//	ThisActor = CursorHit.GetActor();
//
//	if (LastActor == nullptr)
//	{
//		if (ThisActor != nullptr)
//		{
//			ThisActor->HighlightActor();
//		}
//		else
//		{
//			// Both actors are null, do nothing
//		}
//	}
//	else   // LastActor is valid
//	{
//		if (ThisActor == nullptr)
//		{
//			LastActor->UnHighlightActor();
//		}
//		else   // both actors are valid
//		{
//			if (LastActor != ThisActor)
//			{
//				LastActor->UnHighlightActor();
//				ThisActor->HighlightActor();
//			}
//			else
//			{
//				// they are same actor, do nothing
//			}
//		}
//	}
//}
