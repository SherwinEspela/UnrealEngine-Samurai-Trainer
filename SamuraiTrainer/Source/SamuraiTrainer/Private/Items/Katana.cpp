// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Items/Katana.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"

#define SOCKET_FX_SWORDCLASH FName("SocketFXSwordClash")
#define SOCKET_FX_BLOODSPILL FName("SocketFXBloodSpill")

AKatana::AKatana()
{
	FXSwordClash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FX Sword Clash"));
	FXSwordClash->SetupAttachment(Mesh, SOCKET_FX_SWORDCLASH);
	FXSwordClash->SetWorldScale3D(FXSwordClashScale);

	FXSwordClash2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FX Sword Clash 2"));
	FXSwordClash2->SetupAttachment(Mesh, SOCKET_FX_SWORDCLASH);
	FXSwordClash2->SetWorldScale3D(FXSwordClashScale);

	FXBloodSpill = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FX Blood Spill"));
	FXBloodSpill->SetupAttachment(Mesh, SOCKET_FX_BLOODSPILL);
}

void AKatana::BeginPlay()
{
	Super::BeginPlay();

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	if (FXSwordClash && FXSwordClash2)
	{
		FXSwordClash->Deactivate();
		FXSwordClash2->Deactivate();

		FXSwordClash->AttachToComponent(Mesh, TransformRules, SOCKET_FX_SWORDCLASH);
		FXSwordClash->SetWorldScale3D(FXSwordClashScale);

		FXSwordClash2->AttachToComponent(Mesh, TransformRules, SOCKET_FX_SWORDCLASH);
		FXSwordClash2->SetWorldScale3D(FXSwordClashScale);

		FXSwordClashList.Add(FXSwordClash);
		FXSwordClashList.Add(FXSwordClash2);
	}

	if (FXBloodSpill)
	{
		FXBloodSpill->Deactivate();
		FXBloodSpill->AttachToComponent(Mesh, TransformRules, SOCKET_FX_BLOODSPILL);
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

void AKatana::ShouldPlayBloodSpillFx(bool ShouldPlay)
{
	if (FXBloodSpill == nullptr) return;

	if (ShouldPlay)
	{
		FXBloodSpill->Activate(true);
	}
	else {
		FXBloodSpill->Deactivate();
	}
}

void AKatana::PlayParryFatal()
{
	if (MontageKatana)
	{
		Mesh->GetAnimInstance()->Montage_Play(MontageKatana);
	}
}
