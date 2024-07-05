// Super Copyright


#include "Actor/MagicCircle.h"

AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;

	MagicCircalDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("魔法阵贴花"));
	MagicCircalDecal->SetupAttachment(GetRootComponent());
}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

