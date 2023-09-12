


#include "Player/AuraPlayerController.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/GameInputComponent.h"
#include "Interaction/Interactable.h"
#include "Subsystems/WorldCombatDeveloperSettings.h"
#include "Subsystems/WorldCombatSystem.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	ClickToMoveSpline = CreateDefaultSubobject<USplineComponent>("ClickToMoveSpline");
}

void AAuraPlayerController::AutoRunAlongSpline()
{
	const auto Pawn = GetPawn();
	if (!Pawn || !bAutoRunning) return;
	
	const FVector LocationOnSpline = ClickToMoveSpline->
		FindLocationClosestToWorldLocation(Pawn->GetActorLocation(), ESplineCoordinateSpace::World);

	const FVector Direction = ClickToMoveSpline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);

	Pawn->AddMovementInput(Direction);
	const float DistanceToDest = (LocationOnSpline - TargetDestination).Length();

	if(DistanceToDest <= AutoRunAcceptanceRadius)
	{
		bAutoRunning = false;
	}
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	AutoRunAlongSpline();
}

void AAuraPlayerController::CreateDamageNumberPopup_Implementation(AActor* TargetActor, const float DamageAmount,
	const bool bWasCrit, const bool bWasBlock)
{
	if(!IsValid(TargetActor) || !IsLocalController()) return;
	auto DamageComponentType = GetWorld()->GetSubsystem<UWorldCombatSystem>()->GetCombatTextWidgetClass();
	check(DamageComponentType);

	const auto DamageText = NewObject<UDamageTextComponent>(TargetActor, DamageComponentType);
	DamageText->RegisterComponent();
	//Trick to get it to spawn @ the target character.
	DamageText->AttachToComponent(TargetActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	const auto CombatParams = FCombatWidgetDisplayParameters(DamageAmount, bWasCrit, bWasBlock);

	DamageText->SetDamageText(CombatParams);
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	//Input Setup -- For the local player.
	if (auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputSubsystem->AddMappingContext(AuraContext, 0);
	}

	//Cursor setup
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const auto EnhancedInputComponent = CastChecked<UGameInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ThisClass::ShiftPressed);
	EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ThisClass::ShiftReleased);
	
	EnhancedInputComponent->BindAbilityActions(
		InputConfig,
		this,
		&ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased,
		&ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::ShiftPressed()
{
	bShiftKeyDown = true;
}

void AAuraPlayerController::ShiftReleased()
{
	bShiftKeyDown = false;
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const auto InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (const auto ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if(!CursorHit.bBlockingHit) return;
	LastInteractable = CurrentInteractable;
	CurrentInteractable = Cast<IInteractable>(CursorHit.GetActor());

	if(LastInteractable == CurrentInteractable) return;
	if(CurrentInteractable.IsValid()) CurrentInteractable->Highlight();
	if(LastInteractable.IsValid()) LastInteractable->UnHighlight();
}

inline void AAuraPlayerController::LazyUpdateASC()
{
	if(LIKELY(AbilitySystemComponent)) return;
	AbilitySystemComponent = Cast<UAuraAbilitySystemComponent>
		(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPawn()));
}

void AAuraPlayerController::AbilityInputTagPressed(const FGameplayTag InputTag)
{
	LazyUpdateASC();
	bAutoRunning = false;
	FollowTime = 0.0f;
	if(!AbilitySystemComponent) return;
	if(InputTag.MatchesTagExact(TAGS::INPUT::LeftMouseButton))
	{
		bTargeting = CurrentInteractable.IsValid();
        bAutoRunning = false;
	}
}

void AAuraPlayerController::ClickToMovePathed()
{
	const auto Pawn = GetPawn();
	if(!Pawn) return;

	const auto Path = UNavigationSystemV1::FindPathToLocationSynchronously(
		this,
		Pawn->GetActorLocation(),
		TargetDestination);
	if(!Path) return;

	ClickToMoveSpline->ClearSplinePoints();
	for (const auto& Point : Path->PathPoints)
	{
		ClickToMoveSpline->AddSplinePoint(Point, ESplineCoordinateSpace::World);
	}

	if (Path->PathPoints.Num() <= 0)
	{
		ClickToMoveSpline->ClearSplinePoints();
		bAutoRunning = false;
		return;
	}
	
	TargetDestination = Path->PathPoints[Path->PathPoints.Num()-1];
	bAutoRunning = true;
}

void AAuraPlayerController::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	if(!AbilitySystemComponent) return;
	AbilitySystemComponent->AbilityInputTagReleased(InputTag);
	if(!InputTag.MatchesTagExact(TAGS::INPUT::LeftMouseButton) || bTargeting || bShiftKeyDown)
	{
		return;
	}
	
	if (FollowTime <= ShortPressThreshold)
	{
		ClickToMovePathed();
	}

	FollowTime = 0.0f;
	bTargeting = false;
}

void AAuraPlayerController::ClickToMoveHeld()
{
	FollowTime += GetWorld()->GetDeltaSeconds();
	bAutoRunning = false;

	if (!CursorHit.IsValidBlockingHit()) return;
	if(const auto Pawn = GetPawn())
	{
		TargetDestination = CursorHit.ImpactPoint;
		const FVector WorldDirection = (TargetDestination - Pawn->GetActorLocation()).GetSafeNormal();
		Pawn->AddMovementInput(WorldDirection);
	}
}

void AAuraPlayerController::AbilityInputTagHeld(const FGameplayTag InputTag)
{
	if(!AbilitySystemComponent) return;
	if(!InputTag.MatchesTagExact(TAGS::INPUT::LeftMouseButton) || bTargeting || bShiftKeyDown)
	{
		AbilitySystemComponent->AbilityInputTagHeld(InputTag);
		return;
	}

	ClickToMoveHeld();
}
