


#include "AbilitySystem/CustomCalcs/DamageExecCalc.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "GameAbilityTypes.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/GameAbilitySystemLibrary.h"

struct GameDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHit)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalMultiplier)

	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance)

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDef;
	GameDamageStatics() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHit, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalMultiplier, Source, false)
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false)

		TagsToCaptureDef.Add(TAGS::ATTRIBUTES::SECONDARY::RESISTANCE::ResistancePhysical, PhysicalResistanceDef);
		TagsToCaptureDef.Add(TAGS::ATTRIBUTES::SECONDARY::RESISTANCE::ResistanceFire, FireResistanceDef);
		TagsToCaptureDef.Add(TAGS::ATTRIBUTES::SECONDARY::RESISTANCE::ResistanceArcane, ArcaneResistanceDef);
		TagsToCaptureDef.Add(TAGS::ATTRIBUTES::SECONDARY::RESISTANCE::ResistanceLightning, LightningResistanceDef);
	}
};

static const GameDamageStatics DamageStatics()
{
	static GameDamageStatics DamageStatic;
	return DamageStatic;
}

UDamageExecCalc::UDamageExecCalc()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalMultiplierDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
}

template<typename TStatic, typename TParam>
float CalculateAttribute(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	TParam& EvaluationParameters,
	const TStatic& AttributeDef)
{
	float AttributeValue = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		AttributeDef,
		EvaluationParameters,
		AttributeValue
	);
	AttributeValue = FMath::Max(0.f, AttributeValue);
	return AttributeValue;
}

void UDamageExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                             FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const auto Spec = ExecutionParams.GetOwningSpec();
	const auto SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const auto TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const auto SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const auto TargetAvatar = TargetASC ? SourceASC->GetAvatarActor() : nullptr;

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	const float TargetArmor = CalculateAttribute(
		ExecutionParams,
		EvaluationParameters,
		DamageStatics().ArmorDef);
	const float TargetBlockChance = CalculateAttribute(
		ExecutionParams,
		EvaluationParameters,
		DamageStatics().BlockChanceDef);
	const float SourceArmorPen = CalculateAttribute(
		ExecutionParams,
		EvaluationParameters,
		DamageStatics().ArmorPenetrationDef);
	const float SourceCriticalHit = CalculateAttribute(
		ExecutionParams,
		EvaluationParameters,
		DamageStatics().CriticalHitDef);
	const float SourceCriticalMult = CalculateAttribute(
		ExecutionParams,
		EvaluationParameters,
		DamageStatics().CriticalMultiplierDef);
	
	float Damage = 0.0f;
	FGameplayTagContainer DamageTags = FGameplayTagContainer();
	for (auto &[DamageTag, ResistanceTag] : UGameAbilitySystemLibrary::GetCombatDamageResistanceMap())
	{
		const float TargetResistance = CalculateAttribute(
			ExecutionParams,
			EvaluationParameters,
			*DamageStatics().TagsToCaptureDef.Find(ResistanceTag));
		Damage += Spec.GetSetByCallerMagnitude(DamageTag, false, 0.0f) * TargetResistance;
	}
	
	const bool bWasBlocked = FMath::FRandRange(0.f, 1.0f) < TargetBlockChance;
	const bool bWasCritical = FMath::FRandRange(0.f, 1.0f) < SourceCriticalHit;
	const float ModulatedArmor = SourceArmorPen * TargetArmor;

	Damage = bWasBlocked ? Damage / 2.f : Damage;
	Damage -= ModulatedArmor;
	Damage = bWasCritical ? Damage * SourceCriticalMult : Damage;

	auto EffectContextHandle = Spec.GetContext();
	UGameAbilitySystemLibrary::SetBlockedHit(EffectContextHandle, bWasBlocked);
	UGameAbilitySystemLibrary::SetCriticalHit(EffectContextHandle, bWasCritical);
	
	Damage = FMath::Max(0.f, Damage);
	const auto EvalData = FGameplayModifierEvaluatedData(
		UAuraAttributeSet::GetMetaIncomingDamageAttribute(),
		EGameplayModOp::Additive,
		Damage);

	OutExecutionOutput.AddOutputModifier(EvalData);
}
