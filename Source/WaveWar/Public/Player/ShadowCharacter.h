// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "Player/CharacterBase.h"
#include "Types/TurningInPlace.h"
#include "ShadowCharacter.generated.h"



class UAnimMontage;


UCLASS(config = Game)
class WAVEWAR_API AShadowCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AimAnimation")
	float AO_Yaw;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AimAnimation")
	float NewAO_Yaw;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AimAnimation")
	float AO_Pitch;

	FRotator StartingAimRotation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TurningInPlace")
	ETurningInPlace TurningInPlace;

	////****	FUNCTIONS	****////

	AShadowCharacter();

	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return SpringArm; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Begin AActor Interface **/
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;

protected:

	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = Projectile)
	UAnimMontage* ShotMontage;

	FTimerHandle ShotTimer;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector HitTarget;

	////****	FUNCTIONS	****////

	virtual void BeginPlay() override;

	/** Called for aim offset */
	void AimOffset(float DeltaTime);

	/** Called for turn character in place */
	void TurnInPlace(float DeltaTime);

	virtual FVector GetCameraLocation() override;

	void TraceUnderCrosshair(FHitResult& HitResult);

	///** Called for shooting input */
	//void GunShoot();

	///** Called for TimerManager of shooting function */
	//void GunShoot_TimerManager();

private:

	////****	FUNCTIONS	****////

	virtual void InitAbilityActorInfo() override;
};
