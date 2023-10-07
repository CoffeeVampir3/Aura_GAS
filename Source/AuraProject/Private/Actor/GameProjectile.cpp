


#include "Actor/GameProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/GameAbilitySystemLibrary.h"
#include "AuraProject/AuraProject.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AGameProjectile::AGameProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);

	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetSphereRadius(32.0);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void AGameProjectile::OnImpact()
{
	if(TravelingSoundComponent)
	{
		TravelingSoundComponent->Stop();
	}
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
}

void AGameProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!DamageEffectSpecHandle.Data.IsValid()) return;

	const auto TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
	const auto EffectCauser = DamageEffectSpecHandle.Data.Get()->GetEffectContext().GetEffectCauser();
	const auto SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(EffectCauser);

	//Both components are valid, return if they are both equal, or if they are friends.
	if(TargetASC && SourceASC && (TargetASC == SourceASC ||
		!UGameAbilitySystemLibrary::AreNotFriends(SourceASC, TargetASC)))
	{
		return;
	}
		
	OnImpact();

	if(!HasAuthority())
	{
		bHit = true;
		return;
	}

	if(TargetASC)
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
	}

	Destroy();
}

void AGameProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);

	TravelingSoundComponent = UGameplayStatics::SpawnSoundAttached(TravelingSound, GetRootComponent());
}

void AGameProjectile::Destroyed()
{
	if(!bHit && !HasAuthority())
	{
		OnImpact();
	}
	
	Super::Destroyed();
}
