// Uroš Pörš All Rights Reserved


#include "Game/ShadowGameMode.h"
#include "GAS/ShadowAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "Player/ShadowEnemy.h"
#include "Actor/EnemySpawnPoint.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EngineUtils.h"





void AShadowGameMode::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

}

void AShadowGameMode::StartPlay()
{
	Super::StartPlay();

	// Timer to spawn in enemy
	GetWorldTimerManager().SetTimer(SpawnEnemyTimer, this, &AShadowGameMode::SpawnEnemyInInterval, SpawnTimerInterval, true);

	// Timer to spawn in Archer enemy from EnemySpawnPoint
	GetWorldTimerManager().SetTimer(SpawnArcherEnemyTimer, this, &AShadowGameMode::SpawnFromPoint, SpawnTimerInterval, true);
}

void AShadowGameMode::SpawnEnemyInInterval()
{
	// Run EQS_AllPlayerContext in blueprint
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnEnemyQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AShadowGameMode::OnQueryInstanceCompleted);
	}
}

void AShadowGameMode::OnQueryInstanceCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	// If QueryStatus don´t succeed then return
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Enemy EQS Query failed!"));
		return;
	}

	int32 NumOfAliveEnemys = 0;
	for (TActorIterator<AShadowEnemy> It(GetWorld()); It; ++It)
	{
		AShadowEnemy* Enemy = *It;

		// Count alive enemy in level
		if ((Enemy->Execute_GetCharacterClass(Enemy) == ECharacterClass::Fighter) && !Enemy->Execute_IsPlayerDead(Enemy))
		{
			NumOfAliveEnemys++;
		}
	}

	float MaxEnemyCount = 10.0f;
	// If set CurveFloat in editor set MaxEnemyCount as EnemySpawnCurve
	if (EnemySpawnCurve)
	{
		MaxEnemyCount = EnemySpawnCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumOfAliveEnemys >= MaxEnemyCount)
	{
		return;
	}

	// If the query generated Actors the the array is filled with their locations --> Spawn EnemyClass at that location.
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(EnemyClassToSpawn, Locations[0], FRotator::ZeroRotator);
	}
}

void AShadowGameMode::SpawnFromPoint()
{
	int32 NumOfAliveEnemys = 0;
	for (TActorIterator<AShadowEnemy> It(GetWorld()); It; ++It)
	{
		AShadowEnemy* Enemy = *It;


		if ((Enemy->Execute_GetCharacterClass(Enemy) == ECharacterClass::Archer) && !Enemy->Execute_IsPlayerDead(Enemy))
		{
			NumOfAliveEnemys++;
		}
	}

	int32 MaxSpawnRangeAttackers = 4;

	if (NumOfAliveEnemys >= MaxSpawnRangeAttackers)
	{
		return;
	}

	for (AEnemySpawnPoint* Point : EnemySpawnPoint)
	{
		if (IsValid(Point))
		{
			Point->SpawnEnemy();
		}
	}

	/*
	TODO:
		- klièi SpawnEnemy za Archer drugje
	
	*/
}

void AShadowGameMode::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AShadowGameMode* ShadowGM = Cast<AShadowGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (ShadowGM == nullptr)
		return;

	/** Source object */
	AActor* AvatarActor = ASC->GetAvatarActor();

	/** Get data from UCharacterClassData() class. */
	UCharacterClassData* CharacterClassData = ShadowGM->CharacterClassData;
	FCharacterClassInfo ClassInfo = CharacterClassData->GetCharacterClassInfo(CharacterClass);
	
	/** Make effect context --> Make outgoing spec that is ready to be apply --> And apply effect to self (FOR PRIMARY ATTRIBUTES) */
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	/** Make effect context --> Make outgoing spec that is ready to be apply --> And apply effect to self (FOR SECONDARY ATTRIBUTES) */
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassData->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	/** Make effect context --> Make outgoing spec that is ready to be apply --> And apply effect to self (FOR LIFE ATTRIBUTES) */
	FGameplayEffectContextHandle LifeAttributesContextHandle = ASC->MakeEffectContext();
	LifeAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle LifeAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassData->LifeAttributes, Level, LifeAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*LifeAttributesSpecHandle.Data.Get());
}

void AShadowGameMode::GiveStarupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	AShadowGameMode* ShadowGM = Cast<AShadowGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (ShadowGM == nullptr)
		return;

	/** Get data from UCharacterClassData() class. */
	UCharacterClassData* CharacterClassData = ShadowGM->CharacterClassData;

	if (CharacterClassData == nullptr)
		return;

	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassData->Abilities)
	{
		/** Loop through Abilities array from UCharacterClassData and grants an ability. */
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	const FCharacterClassInfo& ClassInfo = CharacterClassData->GetCharacterClassInfo(CharacterClass);

	for (TSubclassOf<UGameplayAbility> AbilityClass : ClassInfo.StartAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			/** Loop through Abilities array from UCharacterClassData and grants an startup ability. */
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

void AShadowGameMode::GetLivePlayersInRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorToIgnore, float Radius, const FVector& SphereLocation)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorToIgnore);

	// query scene to see what we hit
	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		/** Find live character and save in array Overlaps */
		World->OverlapMultiByObjectType(Overlaps, SphereLocation, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			/** Loop through Overlap array to get live overlapping Actors and add to OutOverlappingActors */
			bool CombatInterface = Overlap.GetActor()->Implements<UCombatInterface>();
			if (CombatInterface)
			{
				bool IsActorAlive = !ICombatInterface::Execute_IsPlayerDead(Overlap.GetActor());
				if (IsActorAlive)
				{
					OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
				}
			}
		}
	}
}

void AShadowGameMode::PlayerDied(ACharacter* DeadCharacter)
{


	UGameplayStatics::OpenLevel(DeadCharacter, FName("MainMenu"));
}
