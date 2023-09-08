#pragma once
#include "GameplayEffectTypes.h"
#include "GameAbilityTypes.generated.h"

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