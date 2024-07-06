// Super Copyright


#include "Actor/MagicCircle.h"

AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("魔法阵贴花"));
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagicCircle::SetMaterial(UMaterialInterface* InMaterial)
{
	if (MagicCircleDecal && InMaterial)
	{
		MagicCircleMaterial = InMaterial;
		MagicCircleDecal->SetDecalMaterial(InMaterial);
	}
}

