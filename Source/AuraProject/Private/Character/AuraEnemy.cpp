


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/GameAbilitySystemLibrary.h"
#include "AuraProject/AuraProject.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::Highlight()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_ENEMY_HIGHLIGHT_VALUE);
	Weapon->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_ENEMY_HIGHLIGHT_VALUE);
}

void AAuraEnemy::UnHighlight()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::Die()
{
	SetLifeSpan(5.f);
	
	Super::Die();
}

void AAuraEnemy::BroadcastInitialAttributeValues()
{
	OnMaxHealthChanged(AttributeSet->GetMaxHealth());
	OnHealthChanged(AttributeSet->GetHealth());
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	InitializeAbilityActorInfo();

	AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData NewData)
	{
		OnHealthChanged(NewData.NewValue);
	});
	
	AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData NewData)
	{
		OnMaxHealthChanged(NewData.NewValue);
	});

	auto HitReactChangeDelegate = AbilitySystemComponent->RegisterGameplayTagEvent(
		TAGS::AnimationStatus::AnimationHitReact, EGameplayTagEventType::NewOrRemoved);

	HitReactChangeDelegate.AddLambda([this](const FGameplayTag CallbackTag, int32 NewCount)
	{
		bHitReacting = NewCount > 0;
		GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	});
	
	BroadcastInitialAttributeValues();
}

void AAuraEnemy::InitializeAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	UGameAbilitySystemLibrary::InitializeCharacterDefaultEffects(this,
		CharacterClass, static_cast<float>(StartingLevel), AbilitySystemComponent);
	UGameAbilitySystemLibrary::InitializeCharacterDefaultAbilities(
	this, static_cast<float>(StartingLevel), AbilitySystemComponent);
	AbilitySystemComponent->AbilityActorInfoSet();
}
