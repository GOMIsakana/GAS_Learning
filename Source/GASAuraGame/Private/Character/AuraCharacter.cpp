// Super Copyright


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 900.f;
	CameraSpringArm->bUsePawnControlRotation = false;
	CameraSpringArm->bInheritPitch = false;
	CameraSpringArm->bInheritRoll = false;
	CameraSpringArm->bInheritYaw = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

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

	// 在 服务器上 初始化角色能力信息(AbilityActorInfo)
	// 如果其他没有PossessBy函数的Actor中设置ASC(AbilitySystemComponent)的OnwerActor,需要手动给改Acotr设置一个有效的Controller作为Onwer，以保证ASC的Mixed网络模式正常运行
	InitAbilityActorInfo();
	InitializeDefaultAttribute();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	// 父类中创建后才会创建PlayerState，所以客户端会在这里自动执行InitAbilityActorInfo
	Super::OnRep_PlayerState();

	// 在 客户端上 初始化角色能力信息(AbilityActorInfo)
	InitAbilityActorInfo();

	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	UAuraAbilitySystemComponentBase* AuraASC = Cast<UAuraAbilitySystemComponentBase>(GetAbilitySystemComponent());
	if (AuraPlayerState && AuraASC)
	{
		AuraASC->UpdateAbilityStatuses(AuraPlayerState->GetCombatLevel());
	}
}

void AAuraCharacter::AddToXP_Implementation(float InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState)
	{
		AuraPlayerState->SetXP(AuraPlayerState->GetXP() + InXP);
	}
}

float AAuraCharacter::GetXP_Implementation()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState)
	{
		return AuraPlayerState->GetXP();
	}
	return 0.0f;
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState)
	{
		FLevelUpData tempval = AuraPlayerState->LevelUpInfo->FindLevelDataForExp(Level);
		return tempval.RewardAttributePoint;
	}
	return 0;
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState)
	{
		return AuraPlayerState->GetAttributePoint();
	}
	return 0;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState)
	{
		return AuraPlayerState->LevelUpInfo->FindLevelDataForExp(Level).RewardSpellPoint;
	}
	return 0;
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState)
	{
		return AuraPlayerState->GetSpellPoint();
	}
	return 0;
}

int32 AAuraCharacter::FindLevelForXP_Implementation(float InXP) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState && AuraPlayerState->LevelUpInfo)
	{
		return AuraPlayerState->LevelUpInfo->FindLevelDataForExp(InXP).TargetLevel;
	}
	return 0;
}

void AAuraCharacter::AddToCombatLevel_Implementation(int32 InCombatLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState)
	{
		AuraPlayerState->SetCombatLevel(AuraPlayerState->GetCombatLevel() + InCombatLevel);
		if (UAuraAbilitySystemComponentBase* AuraASC = Cast<UAuraAbilitySystemComponentBase>(GetAbilitySystemComponent()))
		{
			AuraASC->UpdateAbilityStatuses(AuraPlayerState->GetCombatLevel());
		}
	}
}

void AAuraCharacter::AddToAttributePointsReward_Implementation(int32 InAttributePoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState)
	{
		AuraPlayerState->SetAttributePoint(AuraPlayerState->GetAttributePoint() + InAttributePoints);
	}
}

void AAuraCharacter::AddToSpellPointsReward_Implementation(int32 InSpellPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState)
	{
		AuraPlayerState->SetSpellPoint(AuraPlayerState->GetSpellPoint() + InSpellPoints);
	}
}

void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

int32 AAuraCharacter::GetCombatLevel_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState)
	{
		return AuraPlayerState->GetCombatLevel();
	}
	return 0;
}

void AAuraCharacter::OnStunTagChanged(FGameplayTag ReceivedTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	if (bIsStunned)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AAuraCharacter::OnRep_IsStunned()
{
	if (UAuraAbilitySystemComponentBase* AuraASC = Cast<UAuraAbilitySystemComponentBase>(GetAbilitySystemComponent()))
	{
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		FGameplayTagContainer BlockTags;
		BlockTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockTags.AddTag(GameplayTags.Player_Block_InputReleased);
		if (bIsStunned)
		{
			AuraASC->AddLooseGameplayTags(BlockTags);
		}
		else
		{
			AuraASC->RemoveLooseGameplayTags(BlockTags);
		}
	}
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (!IsValid(AuraPlayerState)) return;
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);

	Cast<UAuraAbilitySystemComponentBase>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnASCRegistered.Broadcast(AbilitySystemComponent);

	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Type_Stun).AddUObject(this, &AAuraCharacter::OnStunTagChanged);

	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = FollowCamera->GetComponentLocation();
		const FVector NiagaraComponentLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraComponentLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
	if (LevelUpSoundCue)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LevelUpSoundCue, GetActorLocation());
	}
}
