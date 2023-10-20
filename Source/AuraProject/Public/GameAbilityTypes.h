#pragma once
#include "GameplayEffectTypes.h"
#include "GameAbilityTypes.generated.h"

class UAuraAbilitySystemComponent;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;;

	UPROPERTY()
	float BaseDamage = 0.0f;

	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY()
	float DebuffChance = 0.0f;

	UPROPERTY()
	float DebuffDamage = 0.0f;

	UPROPERTY()
	float DebuffDuration = 0.0f;

	UPROPERTY()
	float DebuffFrequency = 0.0f;
};

USTRUCT(BlueprintType)
struct FCustomEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
public:
	
	bool IsCriticalHit() const {return bIsCriticalHit;}
	void SetIsCriticalHit(const bool bCrit) {bIsCriticalHit=bCrit;}
	bool IsBlockedHit() const { return bIsBlockedHit;}
	void SetIsBlockedHit(const bool bBlocked) {bIsBlockedHit=bBlocked;}

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

protected:
	virtual UScriptStruct* GetScriptStruct() const override {return FCustomEffectContext::StaticStruct(); }

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsBlockedHit = false;
};

template<>
struct TStructOpsTypeTraits< FCustomEffectContext > : TStructOpsTypeTraitsBase2< FCustomEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};