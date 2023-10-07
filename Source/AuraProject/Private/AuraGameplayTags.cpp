


#include "AuraGameplayTags.h"
#include "NativeGameplayTags.h"

namespace TAGS::DATA {
 UE_DEFINE_GAMEPLAY_TAG(Data, "Data")
 UE_DEFINE_GAMEPLAY_TAG(Level, "Data.Level")
}

namespace TAGS::STATUS
{
 UE_DEFINE_GAMEPLAY_TAG(Status, "Status")
 UE_DEFINE_GAMEPLAY_TAG(Dead, "Status.Dead")
}

namespace TAGS::TEAM
{
 UE_DEFINE_GAMEPLAY_TAG(Team, "Team")
 UE_DEFINE_GAMEPLAY_TAG(Player, "Team.Player")
 UE_DEFINE_GAMEPLAY_TAG(Enemy, "Team.Enemy")
}

namespace TAGS::ABILITIES::COMBAT
{
 UE_DEFINE_GAMEPLAY_TAG(Combat, "Abilities.Combat")
 UE_DEFINE_GAMEPLAY_TAG(Attack, "Abilities.Combat.Attack")
 UE_DEFINE_GAMEPLAY_TAG(Summon, "Abilities.Combat.Summon")
}

namespace TAGS::COOLDOWN
{
 UE_DEFINE_GAMEPLAY_TAG(Cooldown, "Cooldown")
 namespace FIRE
 {
  UE_DEFINE_GAMEPLAY_TAG(Fire, "Cooldown.Fire")
  UE_DEFINE_GAMEPLAY_TAG(Firebolt, "Cooldown.Fire.Firebolt")
 }
}

namespace TAGS::ABILITYIDS
{
 UE_DEFINE_GAMEPLAY_TAG(AbilityIds, "AbilityIds")
 namespace FIRE
 {
  UE_DEFINE_GAMEPLAY_TAG(Fire, "AbilityIds.Fire")
  UE_DEFINE_GAMEPLAY_TAG(Firebolt, "AbilityIds.Fire.Firebolt")
 }
}

namespace TAGS::EVENT::COMBATSOCKET
{
 UE_DEFINE_GAMEPLAY_TAG(CombatSocket, "Event.CombatSocket")
 UE_DEFINE_GAMEPLAY_TAG(RightHand, "Event.CombatSocket.RightHand")
 UE_DEFINE_GAMEPLAY_TAG(LeftHand, "Event.CombatSocket.LeftHand")
 UE_DEFINE_GAMEPLAY_TAG(Weapon, "Event.CombatSocket.Weapon") 
}

namespace TAGS::EVENT::MONTAGE::ATTACK
{
 UE_DEFINE_GAMEPLAY_TAG(Attack, "Event.Montage.Attack")
 UE_DEFINE_GAMEPLAY_TAG(One, "Event.Montage.Attack.1")
 UE_DEFINE_GAMEPLAY_TAG(Two, "Event.Montage.Attack.2")
 UE_DEFINE_GAMEPLAY_TAG(Three, "Event.Montage.Attack.3")
 UE_DEFINE_GAMEPLAY_TAG(Four, "Event.Montage.Attack.4")
}

namespace TAGS::DAMAGE::TYPE
{
 UE_DEFINE_GAMEPLAY_TAG(Types, "Damage.Type")
 UE_DEFINE_GAMEPLAY_TAG(Physical, "Damage.Type.Physical")
 UE_DEFINE_GAMEPLAY_TAG(Fire, "Damage.Type.Fire")
 UE_DEFINE_GAMEPLAY_TAG(Arcane, "Damage.Type.Arcane")
 UE_DEFINE_GAMEPLAY_TAG(Lightning, "Damage.Type.Lightning")
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
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(BlockChance, "Attributes.Secondary.BlockChance", "Chance to block incoming attacks")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(CriticalHit, "Attributes.Secondary.CriticalHit", "Chance to deal critical damage")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(CriticalMultiplier, "Attributes.Secondary.CriticalMultiplier", "Multiplier for critical hit damage")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(HealthRegeneration, "Attributes.Secondary.HealthRegeneration", "Rate of health recovery over time")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(ManaRegeneration, "Attributes.Secondary.ManaRegeneration", "Rate of mana recovery over time")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxHealth, "Attributes.Secondary.MaxHealth", "Maximum amount of health")
  UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxMana, "Attributes.Secondary.MaxMana", "Maximum amount of mana")
  
  namespace RESISTANCE
  {
   UE_DEFINE_GAMEPLAY_TAG(ResistanceTypes, "Attributes.Secondary.Resistance.Type")
   UE_DEFINE_GAMEPLAY_TAG(ResistancePhysical, "Attributes.Secondary.Resistance.Physical")
   UE_DEFINE_GAMEPLAY_TAG(ResistanceFire, "Attributes.Secondary.Resistance.Fire")
   UE_DEFINE_GAMEPLAY_TAG(ResistanceArcane, "Attributes.Secondary.Resistance.Arcane")
   UE_DEFINE_GAMEPLAY_TAG(ResistanceLightning, "Attributes.Secondary.Resistance.Lightning")
  }
 }
  
}
