


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
	GameDamageStatics() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHit, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalMultiplier, Source, false)
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
	
	auto DamageTypes = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAGS::DAMAGE::TYPE::Types);

	float Damage = 0.0f;
	FGameplayTagContainer DamageTags = FGameplayTagContainer();
	for (auto& Tag : DamageTypes)
	{
		Damage += Spec.GetSetByCallerMagnitude(Tag, false, 0.0f);
	}

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
