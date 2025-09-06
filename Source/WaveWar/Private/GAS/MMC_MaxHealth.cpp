// Uroš Pörš All Rights Reserved


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

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	return 80.0f + 2.5f * Life + 10.0f * PlayerLevel;
}
