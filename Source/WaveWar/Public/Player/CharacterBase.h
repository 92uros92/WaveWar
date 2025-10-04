// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "Data/CharacterClassData.h"
#include "CharacterBase.generated.h"



class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;


UCLASS(ABSTRACT)
class WAVEWAR_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FMontageForAttack> AttackMontages;

	////****	FUNCTIONS	****////

	ACharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	virtual void Die() override;

	/** That function will run on server and client (NetMulticast) */
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastDeath();

	virtual FVector GetSocketLocation_Implementation(const FGameplayTag& MontageTag) override;

	virtual bool IsPlayerDead_Implementation() const override;

	virtual AActor* GetAvatar_Implementation() override;

	virtual TArray<FMontageForAttack> GetAttackMontages_Implementation() override;

	virtual ECharacterClass GetCharacterClass_Implementation() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Socket")
	FName WeaponSocketName;

	UPROPERTY(EditAnywhere, Category = "Socket")
	FName RightHandName;

	UPROPERTY(EditAnywhere, Category = "Socket")
	FName LeftHandName;

	UPROPERTY(EditAnywhere, Category = "Socket")
	FName GunSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDead;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "PrimaryAttributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SecondaryAttributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SecondaryAttributes")
	TSubclassOf<UGameplayEffect> DefaultLifeAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class Info")
	ECharacterClass CharacterClass = ECharacterClass::Shooter;

	////****	FUNCTIONS	****////

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

	virtual void InitializeDefaultAttributes();

	void AddCharacterAbilities();

private:

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
