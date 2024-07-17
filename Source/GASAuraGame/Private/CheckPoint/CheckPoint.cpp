// Super Copyright


#include "CheckPoint/CheckPoint.h"
#include "Interaction/PlayerInterface.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CheckPointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckPointMesh");
	CheckPointMesh->SetupAttachment(GetRootComponent());
	CheckPointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckPointMesh->SetCollisionResponseToAllChannels(ECR_Block);
	CheckPointMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	Sphere = CreateDefaultSubobject<USphereComponent>("SaveGameRadius");
	Sphere->SetupAttachment(CheckPointMesh);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ACheckPoint::LoadActor_Implementation()
{
	if (bReached)
	{
		HandleCheckPointGlowEffect();
	}
}

void ACheckPoint::HandleCheckPointGlowEffect()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UMaterialInstanceDynamic* MaterialInst = UMaterialInstanceDynamic::Create(CheckPointMesh->GetMaterial(0), this);
	CheckPointMesh->SetMaterial(0, MaterialInst);
	CheckPointReached(MaterialInst);
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnSphereBeginOverlap);
}

void ACheckPoint::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag(FName("Player")))
	{
		if (OtherActor->Implements<UPlayerInterface>())
		{
			bReached = true;
			if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
			{
				AuraGameMode->SaveWorldState(GetWorld());
			}
			/*
			* 此处存档的管线是:
			* 玩家走进检查点范围 -> 检查点调用玩家的存档函数 -> 玩家获取Gamemode中的存档 -> 玩家修改存档中的玩家信息 ->
			* 玩家调用Gamemode中的存档函数 -> Gamemode将修改后的存档中的数据同步到当前GameInstance中(保证当前的信息是正确的), 并保存存档(存档数据会在游戏开始前读取到GameInstance中)
			*/
			IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
			HandleCheckPointGlowEffect();
		}
	}
}
