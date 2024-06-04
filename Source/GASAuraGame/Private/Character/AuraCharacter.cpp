// Super Copyright


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"

AAuraCharacter::AAuraCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
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
}

int32 AAuraCharacter::GetCombatLevel()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState)
	{
		return AuraPlayerState->GetCombatLevel();
	}
	return 0;
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if (!IsValid(AuraPlayerState)) return;
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);

	Cast<UAuraAbilitySystemComponentBase>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
