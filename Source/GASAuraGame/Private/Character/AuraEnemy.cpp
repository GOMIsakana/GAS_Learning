// Super Copyright


#include "Character/AuraEnemy.h"
#include "Debug/DebugDrawComponent.h"
#include "GASAuraGame/GASAuraGame.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AuraAIController.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponentBase>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	BaseWalkSpeed = 250.f;
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	AuraAIController = Cast<AAuraAIController>(NewController);

	if (AuraAIController)
	{
		AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		AuraAIController->RunBehaviorTree(BehaviorTree);
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangerAttacker"), CharacterClass != ECharacterClass::Warrior);
	}
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

int32 AAuraEnemy::GetCombatLevel_Implementation()
{
	return Level;
}

ECharacterClass AAuraEnemy::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

void AAuraEnemy::OnHitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{

	bHitReacting = NewCount > 0;
	if (AuraAIController == nullptr || AuraAIController->GetBlackboardComponent() == nullptr) return;
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	if (bHitReacting || bIsStunned)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AAuraEnemy::OnStunTagChanged(FGameplayTag ReceivedTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	if (AuraAIController == nullptr || AuraAIController->GetBlackboardComponent() == nullptr) return;
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsStunned"), bIsStunned);
	if (bHitReacting || bIsStunned)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

	if (UAuraUserWidget* AuraWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraWidget->SetWidgetController(this);
	}

	BindCallback();
	InitAbilityActorInfo();
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponentBase>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Type_Stun).AddUObject(this, &AAuraEnemy::OnStunTagChanged);

	if (HasAuthority())
	{
		InitializeDefaultAttribute();
	}
	OnASCRegistered.Broadcast(AbilitySystemComponent);
}

void AAuraEnemy::Die(const FVector& DeathImpluse)
{
	SetLifeSpan(LifeSpan);
	if (AuraAIController) AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);

	HealthBar->SetVisibility(false);
	Super::Die(DeathImpluse);
}

void AAuraEnemy::BindCallback()
{
	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	if (AuraAttributeSet == nullptr) return;
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnEnemyHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnEnemyMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Abilities_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,
		&AAuraEnemy::OnHitReactTagChanged
	);
	OnEnemyHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnEnemyMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
}

void AAuraEnemy::InitializeDefaultAttribute() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
