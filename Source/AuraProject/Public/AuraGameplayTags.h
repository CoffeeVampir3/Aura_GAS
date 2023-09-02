

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * 
 */

namespace TAGS::ATTRIBUTES
{

 namespace PRIMARY
 {
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Strength)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Intelligence)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Resilience)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Vigor)
  
  namespace VITAL
  {
   UE_DECLARE_GAMEPLAY_TAG_EXTERN(Health)
   UE_DECLARE_GAMEPLAY_TAG_EXTERN(Mana)
  }
    
 }
 
 namespace SECONDARY
 {
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Armor)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Penetration)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Chance)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(CriticalHit)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(CriticalMultiplier)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(HealthRegeneration)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(ManaRegeneration)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxHealth)
  UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxMana)
 }
  
}
