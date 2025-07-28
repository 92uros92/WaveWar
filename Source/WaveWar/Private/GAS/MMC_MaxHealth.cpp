// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "GAS/MMC_MaxHealth.h"
#include "GAS/ShadowAttributeSet.h"
#include "Interaction/CombatInterface.h"




UMMC_MaxHealth::UMMC_MaxHealth()
{
	/** Get AddMaxHP and configured it */
	LifeDefinition.AttributeToCapture = UShadowAttributeSet::GetAddMaxHPAttribute();
	LifeDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	LifeDefinition.bSnapshot = false;

	/** Added AddMaxHP to Array of CaptureDefinition */
	RelevantAttributesToCapture.Add(LifeDefinition);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Life = 0.0f;
	GetCapturedAttributeMagnitude(LifeDefinition, Spec, EvaluateParameters, Life);
	Life = FMath::Max(Life, 0.0f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.0f + 2.5f * Life + 10.0f * PlayerLevel;
}
