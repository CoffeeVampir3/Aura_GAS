


#include "AuraGameplayTags.h"
#include "NativeGameplayTags.h"

namespace TAGS::ATTRIBUTES
{
 namespace PRIMARY
 {
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(Strength, "Attributes.Primary.Strength", "Increases physical damage and carrying capacity")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(Intelligence, "Attributes.Primary.Intelligence", "Increases magical damage and mana pool")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resilience, "Attributes.Primary.Resilience", "Increases resistance to crowd control and debuffs")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(Vigor, "Attributes.Primary.Vigor", "Increases stamina and health regeneration")

  namespace VITAL
  {
   UE_DEFINE_GAMEPLAY_TAG_COMMENT(Health, "Attributes.Primary.Vital.Health", "Amount of health points")
   UE_DEFINE_GAMEPLAY_TAG_COMMENT(Mana, "Attributes.Primary.Vital.Mana", "Amount of mana points")
  }
    
 }
 
 namespace SECONDARY
 {
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(Armor, "Attributes.Secondary.Armor", "Reduces damage taken, improves block chance")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(Penetration, "Attributes.Secondary.ArmorPenetration", "Ignores a portion of enemy armor")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(Chance, "Attributes.Secondary.BlockChance", "Chance to block incoming attacks")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(CriticalHit, "Attributes.Secondary.CriticalHit", "Chance to deal critical damage")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(CriticalMultiplier, "Attributes.Secondary.CriticalMultiplier", "Multiplier for critical hit damage")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(HealthRegeneration, "Attributes.Secondary.HealthRegeneration", "Rate of health recovery over time")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(ManaRegeneration, "Attributes.Secondary.ManaRegeneration", "Rate of mana recovery over time")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxHealth, "Attributes.Secondary.MaxHealth", "Maximum amount of health")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxMana, "Attributes.Secondary.MaxMana", "Maximum amount of mana")
 }
  
}
