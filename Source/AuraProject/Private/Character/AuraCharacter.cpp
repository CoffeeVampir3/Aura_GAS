


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/GameAbilitySystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


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

	UGameAbilitySystemLibrary::InitializeCharacterDefaultEffects(
this, CharacterClass, static_cast<float>(StartingLevel), AbilitySystemComponent);
	
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Initialize for Client
	InitializeAbilityActorInfo();
}

FVector AAuraCharacter::GetCombatSocketLocation()
{
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AAuraCharacter::InitializeAbilityActorInfo()
{
	Super::InitializeAbilityActorInfo();
	
	auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent());;
	
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	AbilitySystemComponent->AbilityActorInfoSet();
	AttributeSet = Cast<UAuraAttributeSet>(AuraPlayerState->GetAttributeSet());
	
	if (auto AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if(auto AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
