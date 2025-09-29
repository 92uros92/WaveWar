// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/CharacterBase.h"
#include "Types/TurningInPlace.h"
#include "Interaction/ShadowInterface.h"
#include "ShadowCharacter.generated.h"



class UAnimMontage;


UCLASS(config = Game)
class WAVEWAR_API AShadowCharacter : public ACharacterBase, public IShadowInterface
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* LevelUpSound;

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
	virtual int32 GetPlayerLevel_Implementation() override;
	/** END Combat Interface */

	/** Shadow Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;

	virtual void LevelUp_Implementation() override;

	virtual int32 GetXP_Implementation() override;

	virtual int32 FindLevelForXP_Implementation(int32 InXP) override;

	virtual void AddToPlayerLevel_Implementation(int32 InLevelToAdd) override;

	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;

	virtual int32 GetAttributePoints_Implementation(int32 Level) override;
	/** END Shadow Interface */

protected:

	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = Projectile)
	UAnimMontage* ShotMontage;

	FTimerHandle ShotTimer;

	////****	FUNCTIONS	****////

	virtual void BeginPlay() override;

	/** Called for aim offset */
	void AimOffset(float DeltaTime);

	/** Called for turn character in place */
	void TurnInPlace(float DeltaTime);

	virtual FVector GetCameraLocation() override;


	///** Called for shooting input */
	//void GunShoot();

	///** Called for TimerManager of shooting function */
	//void GunShoot_TimerManager();

private:

	////****	FUNCTIONS	****////

	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpSound();
};
