// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Items/Katana.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#define SOCKET_FX_SWORDCLASH FName("SocketFXSwordClash")

AKatana::AKatana()
{
	FXSwordClash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FX Sword Clash"));
	FXSwordClash->SetupAttachment(Mesh, SOCKET_FX_SWORDCLASH);
	FXSwordClash->SetWorldScale3D(FXSwordClashScale);

	FXSwordClash2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FX Sword Clash 2"));
	FXSwordClash2->SetupAttachment(Mesh, SOCKET_FX_SWORDCLASH);
	FXSwordClash2->SetWorldScale3D(FXSwordClashScale);
}

void AKatana::BeginPlay()
{
	Super::BeginPlay();

	if (FXSwordClash && FXSwordClash2)
	{
		FXSwordClash->Deactivate();
		FXSwordClash2->Deactivate();

		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);

		FXSwordClash->AttachToComponent(Mesh, TransformRules, SOCKET_FX_SWORDCLASH);
		FXSwordClash->SetWorldScale3D(FXSwordClashScale);

		FXSwordClash2->AttachToComponent(Mesh, TransformRules, SOCKET_FX_SWORDCLASH);
		FXSwordClash2->SetWorldScale3D(FXSwordClashScale);

		FXSwordClashList.Add(FXSwordClash);
		FXSwordClashList.Add(FXSwordClash2);
	}
}

void AKatana::PlaySwordClashFx()
{
	if (FXSwordClash && FXSwordClash2)
	{
		int RandomNumber = FMath::RandRange(0, FXSwordClashList.Num() - 1);
		UParticleSystemComponent* FXsc = FXSwordClashList[RandomNumber];
		FXsc->Activate(true);
	}
}
