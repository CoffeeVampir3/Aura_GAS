


#include "AbilitySystem/Abilities/ProjectileAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Actor/GameProjectile.h"
#include "Interaction/CombatInterface.h"

void UProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UProjectileAbility::SpawnProjectile(const FVector& TargetLocation)
{
	const auto AvatarActor = GetAvatarActorFromActorInfo();
	if(!AvatarActor->HasAuthority()) return;
	//We're the server.

	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo())) {
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator TargetRot = (TargetLocation - SocketLocation).Rotation();
		TargetRot.Pitch = 0.f;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(TargetRot.Quaternion());
		const auto SpawnedActor = GetWorld()->SpawnActorDeferred<AGameProjectile>(ProjectileClass,
																				  SpawnTransform,
																				  GetOwningActorFromActorInfo(),
																				  Cast<APawn>(GetOwningActorFromActorInfo()),
																				  ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		const auto SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AvatarActor);

		auto EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this);
		const auto SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

		for (const auto& DamageType : DamageMap)
		{
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
				SpecHandle,
				DamageType.Key,
				DamageType.Value.GetValueAtLevel(GetAbilityLevel()));
		}
		
		SpawnedActor->DamageEffectSpecHandle = SpecHandle;
		SpawnedActor->SourceAvatar = AvatarActor;
		SpawnedActor->FinishSpawning(SpawnTransform);
	}
}
