// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "Player/CharacterBase.h"
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

	AShadowCharacter();

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

	////****	FUNCTIONS	****////

	virtual void BeginPlay() override;

	///** Called for shooting input */
	//void GunShoot();

	///** Called for TimerManager of shooting function */
	//void GunShoot_TimerManager();

private:

	virtual void InitAbilityActorInfo() override;
};
