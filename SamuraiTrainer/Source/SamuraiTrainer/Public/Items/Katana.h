// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapon.h"
#include "Katana.generated.h"

class UParticleSystem;
class UParticleSystemComponent;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API AKatana : public AWeapon
{
	GENERATED_BODY()
	
public:
	AKatana();

	virtual void PlaySwordClashFx();

protected:
	virtual void BeginPlay() override;

protected:
	// FX
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystemComponent* FXSwordClash;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystemComponent* FXSwordClash2;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	FVector FXSwordClashScale = FVector(0.5f, 0.5f, 0.5f);

	TArray<UParticleSystemComponent*> FXSwordClashList;
};
