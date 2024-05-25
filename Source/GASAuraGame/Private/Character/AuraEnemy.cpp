// Super Copyright


#include "Character/AuraEnemy.h"
#include "Debug/DebugDrawComponent.h"
#include "GASAuraGame/GASAuraGame.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponentBase>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
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

int32 AAuraEnemy::GetCombatLevel()
{
	return Level;
}

void AAuraEnemy::OnHitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	if (bHitReacting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
	}
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);

	if (UAuraUserWidget* AuraWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraWidget->SetWidgetController(this);
	}

	BindCallback();
	InitAbilityActorInfo();
	InitializeDefaultAttribute();
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponentBase>(AbilitySystemComponent)->AbilityActorInfoSet();
	InitializeDefaultAttribute();
}

void AAuraEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	HealthBar->SetVisibility(false);
	Super::Die();
}

void AAuraEnemy::BindCallback()
{
	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
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
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
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
