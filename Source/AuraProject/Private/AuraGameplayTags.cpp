


#include "AuraGameplayTags.h"
#include "NativeGameplayTags.h"

namespace TAGS::DATA {
 UE_DEFINE_GAMEPLAY_TAG(Data, "Data")
 UE_DEFINE_GAMEPLAY_TAG(Level, "Data.Level")
}

namespace TAGS::DAMAGE::TYPE
{
 UE_DEFINE_GAMEPLAY_TAG(Types, "Damage.Type")
 UE_DEFINE_GAMEPLAY_TAG(Fire, "Damage.Type.Fire")
}

namespace TAGS::AnimationStatus {
 UE_DEFINE_GAMEPLAY_TAG(AnimationStatus, "AnimationStatus")
 UE_DEFINE_GAMEPLAY_TAG_COMMENT(AnimationHitReact, "AnimationStatus.HitReact", "Paused during a hit-react animation")
}

namespace TAGS::INPUT
{
 UE_DEFINE_GAMEPLAY_TAG(Input, "Input")
 UE_DEFINE_GAMEPLAY_TAG(LeftMouseButton, "Input.LeftMouseButton")
 UE_DEFINE_GAMEPLAY_TAG(RightMouseButton, "Input.RightMouseButton")
 UE_DEFINE_GAMEPLAY_TAG(MiddleMouseButton, "Input.MiddleMouseButton")
 
 UE_DEFINE_GAMEPLAY_TAG(Key1, "Input.Keyboard1")
 UE_DEFINE_GAMEPLAY_TAG(Key2, "Input.Keyboard2")
 UE_DEFINE_GAMEPLAY_TAG(Key3, "Input.Keyboard3")
 UE_DEFINE_GAMEPLAY_TAG(Key4, "Input.Keyboard4")
}

namespace TAGS::ATTRIBUTES
{
 UE_DEFINE_GAMEPLAY_TAG(Attribute, "Attributes")
 namespace PRIMARY
 {
  UE_DEFINE_GAMEPLAY_TAG(Primary, "Attributes.Primary")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(Strength, "Attributes.Primary.Strength", "Increases physical damage and carrying capacity")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(Intelligence, "Attributes.Primary.Intelligence", "Increases magical damage and mana pool")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resilience, "Attributes.Primary.Resilience", "Increases resistance to crowd control and debuffs")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(Vigor, "Attributes.Primary.Vigor", "Increases stamina and health regeneration")

  namespace VITAL
  {
   UE_DEFINE_GAMEPLAY_TAG(Vital, "Attributes.Primary.Vital")
   UE_DEFINE_GAMEPLAY_TAG_COMMENT(Health, "Attributes.Primary.Vital.Health", "Amount of health points")
   UE_DEFINE_GAMEPLAY_TAG_COMMENT(Mana, "Attributes.Primary.Vital.Mana", "Amount of mana points")
  }
    
 }
 
 namespace SECONDARY
 {
  UE_DEFINE_GAMEPLAY_TAG(Secondary, "Attributes.Secondary")
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