

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * 
 */

namespace TAGS::DATA
{
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Level)
}

namespace TAGS::STATUS
{
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dead)
}

namespace TAGS::TEAM
{
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Team)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy)
}

namespace TAGS::ABILITIES::COMBAT
{
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combat)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attack)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Summon)
}

namespace TAGS::COOLDOWN
{
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown)
 namespace FIRE
 {
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fire)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Firebolt)
 }
}

namespace TAGS::ABILITYIDS
{
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityIds)
 namespace FIRE
 {
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fire)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Firebolt)
 }
}

namespace TAGS::EVENT::COMBATSOCKET
{
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(RightHand)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(LeftHand)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon) 
}

namespace TAGS::EVENT::MONTAGE::ATTACK
{
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attack)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(One)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Two)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Three)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Four)
}

namespace TAGS::DAMAGE::TYPE
{
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Types)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Physical)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fire)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Arcane)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Lightning)
}

namespace TAGS::AnimationStatus
{
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(AnimationStatus)
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(AnimationHitReact)
}

namespace TAGS::INPUT
{
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(LeftMouseButton)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(RightMouseButton)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(MiddleMouseButton)
  
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Key1)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Key2)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Key3)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Key4)
}

namespace TAGS::ATTRIBUTES
{
 UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute)
 namespace PRIMARY
 {
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Primary)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Strength)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Intelligence)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Resilience)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Vigor)
  
  namespace VITAL
  {
   UE_DECLARE_GAMEPLAY_TAG_EXTERN(Vital)
   UE_DECLARE_GAMEPLAY_TAG_EXTERN(Health)
   UE_DECLARE_GAMEPLAY_TAG_EXTERN(Mana)
  }
    
 }
 
 namespace SECONDARY
 {
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Secondary)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Armor)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Penetration)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(BlockChance)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(CriticalHit)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(CriticalMultiplier)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(HealthRegeneration)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(ManaRegeneration)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxHealth)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxMana)
  
  namespace RESISTANCE
  {
   UE_DECLARE_GAMEPLAY_TAG_EXTERN(ResistanceTypes)
   UE_DECLARE_GAMEPLAY_TAG_EXTERN(ResistancePhysical)
   UE_DECLARE_GAMEPLAY_TAG_EXTERN(ResistanceFire)
   UE_DECLARE_GAMEPLAY_TAG_EXTERN(ResistanceArcane)
   UE_DECLARE_GAMEPLAY_TAG_EXTERN(ResistanceLightning)
  }
 }
  
}
