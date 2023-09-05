


#include "Character/AuraEnemy.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/GameAbilitySystemLibrary.h"
#include "AuraProject/AuraProject.h"
#include "Components/WidgetComponent.h"

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

void AAuraEnemy::BroadcastInitialAttributeValues()
{
	OnMaxHealthChanged(AttributeSet->GetMaxHealth());
	OnHealthChanged(AttributeSet->GetHealth());
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

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

	BroadcastInitialAttributeValues();
}

void AAuraEnemy::InitializeAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	UGameAbilitySystemLibrary::InitializeCharacterDefaultEffects(this,
		CharacterClass, static_cast<float>(StartingLevel), AbilitySystemComponent);
	//AddCharacterAbilities();
	AbilitySystemComponent->AbilityActorInfoSet();
}
