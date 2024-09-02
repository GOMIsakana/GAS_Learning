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
#include "Data/DropItems.h"

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

void AAuraPlayerController::SortBackpackItems_Implementation(bool bAscending)
{
	struct FSortBackpackByBackpackSlot
	{
		FSortBackpackByBackpackSlot(bool bInbAscending) : bAscending(bInbAscending) {};

		bool bAscending = true;

		FORCEINLINE bool operator()(const FBackpackItem& ItemA, const FBackpackItem& ItemB) const
		{
			return bAscending ? ItemA.BackpackSlot < ItemB.BackpackSlot : ItemA.BackpackSlot > ItemB.BackpackSlot;
		}
	};
	BackpackItems.Sort(FSortBackpackByBackpackSlot(bAscending));
}

void AAuraPlayerController::ExchangeItem_Implementation(int32 SourceItemSlot, int32 TargetItemSlot)
{
	FBackpackItem SourceItem;
	FBackpackItem TargetItem;
	GetItemAtBackpackSlot(SourceItem, SourceItemSlot);
	GetItemAtBackpackSlot(TargetItem, TargetItemSlot);
	/**
	* 
	* 拿起物品时, 将原来的物品转化成一个新物品附着到鼠标上, 原来位置清空或者数量减半(左键拿整组, 右键取一半)
	* 如果鼠标落点位置为无效落点, 则弹回物品到原本的物品堆上
	* 不会对空位置做出反应
	* 
	* 下面所说的"交换", 本质上都是从原本的物品堆中取其物品信息创建一个新的widget部件, 将原本鼠标上的物品信息覆盖到物品堆上, 再将新创建的widget绑定到鼠标上
	* 如果物品类型一样, 检查物品的最大堆叠数, 如果未达到堆叠上限, 则合并两个物品, 并将溢出的物品堆叠更新到鼠标上的物品
	* 如果物品类型不一样, 则进行位置交换(目标位置上的物品和鼠标上的物品进行交换)
	* 如果目标没有物品, 则直接放置在目标位置
	* 
	*/

	// 如果物品类型一样, 检查物品的最大堆叠数
	if (SourceItem.ItemTag.MatchesTagExact(TargetItem.ItemTag))
	{
		FItemInfo SourceItemInfo = DropItemAsset->FindItemInfoByItemTag(SourceItem.ItemTag);
		FItemInfo TargetItemInfo = DropItemAsset->FindItemInfoByItemTag(TargetItem.ItemTag);
		BackpackItemUpdateDelegate.Broadcast(SourceItem);
		BackpackItemUpdateDelegate.Broadcast(TargetItem);
	}
}

void AAuraPlayerController::GetItemAtBackpackSlot_Implementation(FBackpackItem& OutItem, int32 BackpackSlot)
{
	for (const FBackpackItem& Item : BackpackItems)
	{
		if (Item.BackpackSlot == BackpackSlot)
		{
			OutItem = Item;
			return;
		}
	}
	OutItem = FBackpackItem();
}

void AAuraPlayerController::SetItemAtBackpackSlot_Implementation(FBackpackItem InItem, int32 InBackpackSlot, bool bRemoveInItemSourceSlotItem)
{
	if (InItem.BackpackSlot == -1) return;	// 背包位置为-1代表不存在, 不做任何处理
	int32 ToRemoveItemIndex = -1;
	int32 InItemArrayIndex = -1;
	int32 InItemOriginBackpackSlot = InItem.BackpackSlot;
	InItem.BackpackSlot = InBackpackSlot;
	// 遍历所有物品
	for (int32 i = 0; i < BackpackItems.Num(); i++)
	{
		FBackpackItem& Item = BackpackItems[i];	// 获取物品
		if (bRemoveInItemSourceSlotItem && Item.BackpackSlot == InItemOriginBackpackSlot)	// 如果有物品位于输入物品的原来位置, 则将其记录下来
		{
			ToRemoveItemIndex = i;
		}
		if (Item.BackpackSlot == InBackpackSlot)	// 如果找到需要覆盖的位置, 则将其覆盖
		{
			Item = InItem;
			BackpackItemUpdateDelegate.Broadcast(InItem);
			InItemArrayIndex = i;
		}
	}
	// 只有当原本位置和目标位置不是同一个物品的时候才会移除掉原本位置的物品
	if (bRemoveInItemSourceSlotItem && ToRemoveItemIndex != -1 && InItemOriginBackpackSlot != InBackpackSlot)
	{
		FBackpackItem ToRemoveItem;
		ToRemoveItem.BackpackSlot = InItemOriginBackpackSlot;
		BackpackItemUpdateDelegate.Broadcast(ToRemoveItem);
		BackpackItems.RemoveAt(ToRemoveItemIndex);
	}
	// 如果已经塞入背包, 则再次不塞入背包
	if (InItemArrayIndex != -1)
	{
		// 如果物品数量为零, 则移除该物品
		if (InItem.ItemAmount <= 0)
		{
			FBackpackItem ToRemoveItem;
			ToRemoveItem.BackpackSlot = BackpackItems[InItemArrayIndex].BackpackSlot;
			BackpackItemUpdateDelegate.Broadcast(ToRemoveItem);
			BackpackItems.RemoveAt(InItemArrayIndex);
		}
		return;
	}
	// 如果没有在对应位置上的物品、且数量大于0, 则直接放入背包, 当新开了一个格子
	if (InItem.ItemAmount > 0)
	{
		BackpackItems.Add(InItem);
		BackpackItemUpdateDelegate.Broadcast(InItem);
	}
}

void AAuraPlayerController::GetItemAtEquipSlot_Implementation(FBackpackItem& OutItem, int32 InEquipSlot)
{
	for (const FBackpackItem& Item : BackpackItems)
	{
		if (Item.EquipSlot == InEquipSlot)
		{
			OutItem = Item;
		}
	}
}

void AAuraPlayerController::EquipItemToSlot_Implementation(int32 ToEquipItemBackpackSlot, int32 InEquipSlot)
{
	// 移除该位置上原有的物品
	for (FBackpackItem& Item : BackpackItems)
	{
		if (Item.EquipSlot == InEquipSlot)
		{
			/*
			Item.Value.EquipSlot = -1;
			// 将应用特效时的effect存到pc中
			AuraAbilitySystemComponent->ApplyGameplayEffectSpecToSelf;
				AuraAbilitySystemComponent->GetActiveGameplayEffect();
			// 需要更改时, 获取当前的SetByCaller值, 减去需要移除的值，并更新到里面
			FGameplayEffectSpecHandle SpecHandle;
			SpecHandle.Data.Get()->GetSetByCallerMagnitude();
			AuraAbilitySystemComponent->UpdateActiveGameplayEffectSetByCallerMagnitude
			// 修改原本的被动effect的值
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude();
			*/
			break;
			
		}
	}

	// 将需要装备的物品装备到槽位上
	for (FBackpackItem& Item : BackpackItems)
	{
		if (Item.BackpackSlot == ToEquipItemBackpackSlot)
		{
			Item.EquipSlot = InEquipSlot;
			break;
		}
	}
}

bool AAuraPlayerController::PickupItem_Implementation(FBackpackItem& InItem)
{
	// 先进行排序, 保证物品栏顺序正确
	// 这里的排序是排序的数组, 跟读取的没有关系
	SortBackpackItems(true);
	// 检测第一个检测到的空位
	int32 FirstEmptySlot = 0;
	for (const FBackpackItem& Item : BackpackItems)
	{
		// 如果0号位置不是空位就一直检查到有空位为止
		if (Item.BackpackSlot == FirstEmptySlot)
		{
			FirstEmptySlot++;
		}
	}
	// 如果溢出了, 那就不拾取了
	if (FirstEmptySlot == GetBackpackSize()) return false;
	InItem.BackpackSlot = FirstEmptySlot;
	// 如果位置没有抵达上限, 则放置在找到的第一个空位
	BackpackItems.Add(InItem);
	// 发送背包更新的提示
	BackpackItemUpdateDelegate.Broadcast(InItem);
	return true;
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
