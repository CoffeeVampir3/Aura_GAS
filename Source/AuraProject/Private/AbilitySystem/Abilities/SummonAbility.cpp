


#include "AbilitySystem/Abilities/SummonAbility.h"

TArray<FVector> USummonAbility::GetSpawnLocations()
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const FVector Location = AvatarActor->GetActorLocation();
	const FVector Forward = AvatarActor->GetActorForwardVector();

	const float DeltaSpread = SpawnSpreadDegrees / SpawnThisMany;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpreadDegrees / 2.f, FVector::UpVector);

	auto SelectedPoints = TArray<FVector>();

	for(int i = 0; i < SpawnThisMany; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector ChosenSpawnPoint = Location + Direction * FMath::FRandRange(SpawnExclusionRadius, SpawnRadius);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnPoint + FVector::UpVector * 500.f,
			ChosenSpawnPoint + FVector::UpVector * -500.f, ECC_Visibility);

		if(Hit.bBlockingHit)
		{
			SelectedPoints.Add(Hit.ImpactPoint);
		}
	}

	return SelectedPoints;
}
