


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"


AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Initialize for Server
	InitializeAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Initialize for Client
	InitializeAbilityActorInfo();
}

void AAuraCharacter::InitializeAbilityActorInfo()
{
	auto PlayerState = GetPlayerState<AAuraPlayerState>();
	check(PlayerState);
	PlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerState, this);
	AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	AttributeSet = PlayerState->GetAttributeSet();
}
