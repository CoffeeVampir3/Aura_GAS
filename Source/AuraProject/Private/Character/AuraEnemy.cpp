


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/GameAbilitySystemLibrary.h"
#include "Ai/BaseAIController.h"
#include "AuraProject/AuraProject.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bHitReacting = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(!HasAuthority()) return;
	//Ai is Server only.
	BaseAiController = Cast<ABaseAIController>(NewController);
	check(BaseAiController);

	BaseAiController->GetBlackboardComponent()->InitializeBlackboard(*BehaviourTree->BlackboardAsset);
	BaseAiController->RunBehaviorTree(BehaviourTree);
	BaseAiController->GetBlackboardComponent()->SetValueAsBool(BT_HitReactingName, false);
	BaseAiController->GetBlackboardComponent()->SetValueAsBool(BT_RangedAttacker, CharacterClass != ECharacterClass::Warrior);
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

void AAuraEnemy::Die()
{
	if(HasAuthority() && BaseAiController && BaseAiController->GetBlackboardComponent())
	{
		BaseAiController->GetBlackboardComponent()->SetValueAsBool(BT_IsDead, true);
	}
	Super::Die();
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	InitializeAbilityActorInfo();

	if(HasAuthority())
	{
		UGameAbilitySystemLibrary::InitializeCharacterDefaultEffects(
			this, CharacterClass, static_cast<float>(StartingLevel), AbilitySystemComponent);
		UGameAbilitySystemLibrary::InitializeCharacterDefaultAbilities(
			this, CharacterClass, static_cast<float>(StartingLevel), AbilitySystemComponent);
	}

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

	AbilitySystemComponent->RegisterGameplayTagEvent(
		TAGS::AnimationStatus::AnimationHitReact, EGameplayTagEventType::NewOrRemoved)
		.AddLambda([this](const FGameplayTag CallbackTag, int32 NewCount)
	{
		bHitReacting = NewCount > 0;
		GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
		if(HasAuthority() && BaseAiController && BaseAiController->GetBlackboardComponent())
		{
			BaseAiController->GetBlackboardComponent()->SetValueAsBool(BT_HitReactingName, bHitReacting);
		}
	});
	
	BroadcastInitialAttributeValues();
}

void AAuraEnemy::InitializeAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->AbilityActorInfoSet();

	AbilitySystemComponent->AddLooseGameplayTag(TAGS::TEAM::Enemy);
}
