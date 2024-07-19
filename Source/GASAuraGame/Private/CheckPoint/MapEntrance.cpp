// Super Copyright


#include "CheckPoint/MapEntrance.h"
#include "Interaction/PlayerInterface.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Sphere->SetupAttachment(MoveToLocationComponent);
}

void AMapEntrance::HighlightActor_Implementation()
{
	CheckPointMesh->SetRenderCustomDepth(true);
}

void AMapEntrance::LoadActor_Implementation()
{
}

void AMapEntrance::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag(FName("Player")))
	{
		if (OtherActor->Implements<UPlayerInterface>())
		{
			bReached = true;
			if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
			{
				AuraGameMode->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
			}

			IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);
		}
		
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}
