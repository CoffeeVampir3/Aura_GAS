#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()
	
	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY()
    AActor* SourceAvatarActor = nullptr;
	UPROPERTY()
	AController* SourceController = nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;
	
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()
	AController* TargetController = nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

/**
 * 
 */
UCLASS()
class AURAPROJECT_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/*
	 * Meta -- NOT Replicated, these are useful only for the server.
	 */

	UPROPERTY(BlueprintReadOnly, Category="Meta Attribute")
	FGameplayAttributeData MetaIncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MetaIncomingDamage);

	/*
	 *	Growth
	 */

#pragma region Level Attribute
	UPROPERTY(BlueprintReadOnly, Category="Growth Attribute|Attribute", ReplicatedUsing = OnRep_Level)
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Level)
	UFUNCTION()
	virtual void OnRep_Level(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Level, OldValue);
	}
#pragma endregion

	/*
	 *
	 */

#pragma region PhysicalResistance Attribute
	UPROPERTY(BlueprintReadOnly, Category="Attributes|Resistances", ReplicatedUsing = OnRep_PhysicalResistance)
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance)
	UFUNCTION()
	virtual void OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldValue);
	}
#pragma endregion

#pragma region FireResistance Attribute
	UPROPERTY(BlueprintReadOnly, Category="Attributes|Resistances", ReplicatedUsing = OnRep_FireResistance)
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance)
	UFUNCTION()
	virtual void OnRep_FireResistance(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldValue);
	}
#pragma endregion

#pragma region ArcaneResistance Attribute
	UPROPERTY(BlueprintReadOnly, Category="Attributes|Resistances", ReplicatedUsing = OnRep_ArcaneResistance)
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneResistance)
	UFUNCTION()
	virtual void OnRep_ArcaneResistance(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldValue);
	}
#pragma endregion

#pragma region LightningResistance Attribute
	UPROPERTY(BlueprintReadOnly, Category="Attributes|Resistances", ReplicatedUsing = OnRep_LightningResistance)
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance)
	UFUNCTION()
	virtual void OnRep_LightningResistance(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldValue);
	}
#pragma endregion

	/*
	 *	Core
	 */

#pragma region Strength Attribute
	UPROPERTY(BlueprintReadOnly, Category="Core Attribute", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength)
	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldValue);
	}
#pragma endregion

#pragma region Intelligence Attribute
	UPROPERTY(BlueprintReadOnly, Category="Core Attribute", ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence)
	UFUNCTION()
	virtual void OnRep_Intelligence(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldValue);
	}
#pragma endregion

#pragma region Resilience Attribute
	UPROPERTY(BlueprintReadOnly, Category="Core Attribute", ReplicatedUsing = OnRep_Resilience)
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience)
	UFUNCTION()
	virtual void OnRep_Resilience(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldValue);
	}
#pragma endregion

#pragma region Vigor Attribute
	UPROPERTY(BlueprintReadOnly, Category="Core Attribute", ReplicatedUsing = OnRep_Vigor)
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor)
	UFUNCTION()
	virtual void OnRep_Vigor(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldValue);
	}
#pragma endregion
	
	/*
	 *	Vitals
	 */

#pragma region Health Attribute
	UPROPERTY(BlueprintReadOnly, Category="Vital Attribute", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health)
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldValue);
	}
#pragma endregion

#pragma region Mana Attribute
	UPROPERTY(BlueprintReadOnly, Category="Vital Attribute", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)
	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldValue);
	}
#pragma endregion
	
	/*
	 *	Secondary Attribute
	 */

#pragma region Armor Attribute
	UPROPERTY(BlueprintReadOnly, Category="Secondary Attribute|Attribute", ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor)
	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldValue);
	}
#pragma endregion

#pragma region ArmorPenetration Attribute
	UPROPERTY(BlueprintReadOnly, Category="Secondary Attribute|Attribute", ReplicatedUsing = OnRep_ArmorPenetration)
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration)
	UFUNCTION()
	virtual void OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldValue);
	}
#pragma endregion

#pragma region BlockChance Attribute
	UPROPERTY(BlueprintReadOnly, Category="Secondary Attribute|Attribute", ReplicatedUsing = OnRep_BlockChance)
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance)
	UFUNCTION()
	virtual void OnRep_BlockChance(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldValue);
	}
#pragma endregion

#pragma region CriticalHit Attribute
	UPROPERTY(BlueprintReadOnly, Category="Secondary Attribute|Attribute", ReplicatedUsing = OnRep_CriticalHit)
	FGameplayAttributeData CriticalHit;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHit)
	UFUNCTION()
	virtual void OnRep_CriticalHit(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHit, OldValue);
	}
#pragma endregion

#pragma region CriticalMultiplier Attribute
	UPROPERTY(BlueprintReadOnly, Category="Secondary Attribute|Attribute", ReplicatedUsing = OnRep_CriticalMultiplier)
	FGameplayAttributeData CriticalMultiplier;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalMultiplier)
	UFUNCTION()
	virtual void OnRep_CriticalMultiplier(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalMultiplier, OldValue);
	}
#pragma endregion

#pragma region HealthRegeneration Attribute
	UPROPERTY(BlueprintReadOnly, Category="Secondary Attribute|Attribute", ReplicatedUsing = OnRep_HealthRegeneration)
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration)
	UFUNCTION()
	virtual void OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldValue);
	}
#pragma endregion

#pragma region ManaRegeneration Attribute
	UPROPERTY(BlueprintReadOnly, Category="Secondary Attribute|Attribute", ReplicatedUsing = OnRep_ManaRegeneration)
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration)
	UFUNCTION()
	virtual void OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldValue);
	}
#pragma endregion
	
#pragma region MaxHealth Attribute
	UPROPERTY(BlueprintReadOnly, Category="Secondary Attribute|Vital Attribute|Attribute", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth)
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldValue);
	}
#pragma endregion

#pragma region MaxMana Attribute
	UPROPERTY(BlueprintReadOnly, Category="Secondary Attribute|Vital Attribute|Attribute", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana)
	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldValue)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldValue);
	}
#pragma endregion

private:
	static FEffectProperties MakeEffectProperties(const FGameplayEffectModCallbackData& Data);
};
