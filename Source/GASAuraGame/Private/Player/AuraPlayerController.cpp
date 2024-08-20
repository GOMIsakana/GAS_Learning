// Super Copyright


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"
#include "GameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Components/WidgetComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GASAuraGame/GASAuraGame.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun(); 
	UpdateMagicCircleLocation();
}

void AAuraPlayerController::ShowMagicCircle()
{
	if (MagicCircle) return;
	MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
}

void AAuraPlayerController::HideMagicCircle()
{
	if (MagicCircle)
	{
		MagicCircle->Destroy();
		MagicCircle = nullptr;
	}
}

void AAuraPlayerController::SetMagicCircleMaterial(UMaterialInterface* InMaterial)
{
	if (MagicCircle && InMaterial)
	{
		MagicCircle->SetMaterial(InMaterial);
	}
}

void AAuraPlayerController::ClientShowDamageNumber_Implementation(float DamageAmount, AActor* Target, bool bCriticalHit, bool bDamageValid)
{
	if (GetPawn() && IsValid(Target) && DamageTextComponentClass)
	{
		UDamageTextWidgetComponent* DamageText = NewObject<UDamageTextWidgetComponent>(GetPawn(), DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(Target->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bCriticalHit, bDamageValid);
	}
}

void AAuraPlayerController::ClientShowFloatingTextNumber_Implementation(float Number, AActor* Target, const FString& TextPrefix, const FString& TextSuffix, FSlateColor ColorOverride)
{
	if (GetPawn() && IsValid(Target) && FloatingTextComponentClass)
	{
		UFloatingTextWidgetComponent* FloatingText = NewObject<UFloatingTextWidgetComponent>(GetPawn(), FloatingTextComponentClass);
		FloatingText->RegisterComponent();
		FloatingText->AttachToComponent(Target->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		FloatingText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		FloatingText->UpdateFloatingText(Number, TextPrefix, TextSuffix, ColorOverride);
	}
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		// 根据自己当前的位置, 找到最接近样条线的位置与方向。然后将方向作为移动的方向进行输入。
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector DirectionOnSpline = Spline->FindDirectionClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(DirectionOnSpline);
		// 获取当前位置到目标位置的距离，如果小于导航可接受的距离，则停止移动
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if (MagicCircle)
	{
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
	}
}

void AAuraPlayerController::CursorTrace()
{
	if (GetAuraASC() && GetAuraASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		return;
	}
	const ECollisionChannel MouseTraceChannel = MagicCircle ? ECC_ExcludePlayer : ECollisionChannel::ECC_Visibility;
	GetHitResultUnderCursor(MouseTraceChannel, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		if (TracingActor)
		{
			UnhighlightActor(TracingActor);
		}
		LastTraceActor = nullptr;
		TracingActor = nullptr;
		TargetingStatus = ETargetingStatus::NoTargeting;
		return;
	}

	LastTraceActor = TracingActor;
	if (IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>())
	{
		TracingActor = CursorHit.GetActor();
		TargetingStatus = TracingActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
	}
	else
	{
		TracingActor = nullptr;
		TargetingStatus = ETargetingStatus::NoTargeting;
	}
	if (TracingActor != LastTraceActor)
	{
		if (TracingActor) HighlightActor(TracingActor);
		if (LastTraceActor) UnhighlightActor(LastTraceActor);
	}
}

void AAuraPlayerController::HighlightActor(AActor* Actor)
{
	if (IsValid(Actor) && Actor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_HighlightActor(Actor);
	}
}

void AAuraPlayerController::UnhighlightActor(AActor* Actor)
{
	if (IsValid(Actor) && Actor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_UnhighlightActor(Actor);
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetAuraASC() && GetAuraASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	if (GetAuraASC()) GetAuraASC()->AbilityInputTagPressed(InputTag);
	// 当按下右键时，检测右键是否正在对准目标，并禁用自动移动(在下方根据长按还是点按重新分配自动移动)
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RMB))
	{
		if (TracingActor)
		{
			TargetingStatus = TracingActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
		}
		else
		{
			TargetingStatus = ETargetingStatus::NoTargeting;
		}
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetAuraASC() && GetAuraASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	// 如果不是右键的输入, 则尝试释放松开按键时的技能
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RMB))
	{
		if (GetAuraASC()) GetAuraASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	// 如果鼠标准星对着目标，或按着shift
	if (GetAuraASC()) GetAuraASC()->AbilityInputTagReleased(InputTag);

	// 如果对准地板且没按下shift，则使用导航系统从当前位置到目标位置生成一条导航线，并获取其中转折点作为spline的点，然后使用spline进行导航
	if (TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftKeyDown)
	{
		// 如果目标重载了被点击位置, 则使用目标的被点击函数提供的位置, 否则使用在Held时实时计算的鼠标落下位置
		if (TracingActor && TracingActor->Implements<UHighlightInterface>())
		{
			IHighlightInterface::Execute_SetMoveToDestination(TracingActor, CachedDestination);
		}
		// 上面重载过后, 会在被点击的位置生成移动箭头的粒子效果
		if (ClickNiagaraSystem && GetAuraASC() && !GetAuraASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
		}
		APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThresHold && ControlledPawn)
		{
			if (UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : Path->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					// DrawDebugSphere(GetWorld(), PointLoc, 10.f, 8, FColor::Red, false, 5.f);
				}
				if (Path->PathPoints.Num())
				{
					CachedDestination = Path->PathPoints[Path->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}
		}

		FollowTime = 0.f;
	}

	if (GetAuraASC() == nullptr) return;
	GetAuraASC()->AbilityInputTagReleased(InputTag);
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetAuraASC() && GetAuraASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RMB))
	{
		if (GetAuraASC()) GetAuraASC()->AbilityInputTagHeld(InputTag);
		return;
	}

	if (TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyDown)
	{
		if (GetAuraASC()) GetAuraASC()->AbilityInputTagHeld(InputTag);
	}
	// 长按时跟着鼠标当前方向走
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponentBase* AAuraPlayerController::GetAuraASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponentBase>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

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

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	/*
	if (MoveAction)
	{
		AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	}
	*/
	if (ShiftAction)
	{
		AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
		AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	}

	if (InputConfig)
	{
		AuraInputComponent->BindAbilityActions(InputConfig, this, &AAuraPlayerController::AbilityInputTagPressed, &AAuraPlayerController::AbilityInputTagReleased, &AAuraPlayerController::AbilityInputTagHeld);
	}
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetAuraASC() && GetAuraASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
