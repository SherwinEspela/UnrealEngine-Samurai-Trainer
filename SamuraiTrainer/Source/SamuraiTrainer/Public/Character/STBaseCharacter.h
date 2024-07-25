// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STBaseCharacter.generated.h"

class AKatana;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackStartedSignature, FName, BlockSectionName);

UCLASS()
class SAMURAITRAINER_API ASTBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASTBaseCharacter();
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

public:
	void ReduceHealth(int Value);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWarpTargetUpdated();

	virtual void PlayAttackStagger(FName SectionName);

public:
	FORCEINLINE bool IsHealthCritical() const { return bIsHealthCritical; }

public:
	FOnAttackStartedSignature OnAttackStarted;

protected:
	virtual void BeginPlay() override;

	void AttachSwordToSocket(FName SocketName);

protected:
	// Character States
	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	float HealthMax = 100;

	bool bIsHealthCritical = false;

	UPROPERTY(EditDefaultsOnly)
	float CriticalHealthAmount = 20.f;

protected:
	// Weapon
	UPROPERTY(EditAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AKatana> KatanaClass;

	AKatana* Katana;

protected:
	// Animations
	UPROPERTY(EditDefaultsOnly, Category = "Animation Montages")
	UAnimMontage* MontageAttackStagger;

};
