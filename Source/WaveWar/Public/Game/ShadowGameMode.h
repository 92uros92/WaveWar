// Uroš Pörš All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/CharacterClassData.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "ShadowGameMode.generated.h"




class UCharacterClassData;
class UAbilitySystemComponent;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class EnvQueryTypes;
class ULootData;
class AEnemySpawnPoint;
class AShadowEnemy;
class USoundWave;
class USpawnMeleeEnemyData;


UCLASS()
class WAVEWAR_API AShadowGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Attributes Class|Character Class")
	TObjectPtr<UCharacterClassData> CharacterClassData;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawn Data")
	TObjectPtr<USpawnMeleeEnemyData> SpawnMeleeEnemyData;

	TArray<struct FEnemyToSpawnInfo> EnemyToSpawnArray;

	UPROPERTY(EditDefaultsOnly, Category = "LootData")
	TObjectPtr<ULootData> LootData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundWave* AmbientSounWave;

	////****	FUNCTIONS	****////

	virtual void Tick(float DeltaSecond) override;

	virtual void StartPlay() override;

	/** Initialize default attributes for enemy, base on CharacterClass and Level */
	UFUNCTION(BlueprintCallable)
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	/** Initialize startup abilities for enemy */
	UFUNCTION(BlueprintCallable)
	static void GiveStarupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable)
	static void GetLivePlayersInRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorToIgnore, float Radius, const FVector& SphereLocation);

	void PlayerDied(ACharacter* DeadCharacter);

protected:

	FTimerHandle SpawnEnemyTimer;

	FTimerHandle SpawnArcherEnemyTimer;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnEnemyQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> EnemyClassToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* EnemySpawnCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* RangeEnemySpawnCurve;

	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<AEnemySpawnPoint*> EnemySpawnPoints;

	////****	FUNCTIONS	****////

	UFUNCTION()
	void SpawnEnemyInInterval();

	UFUNCTION()
	void OnQueryInstanceCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnFromPoint();

};
