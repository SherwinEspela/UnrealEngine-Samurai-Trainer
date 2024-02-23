// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/STBaseCharacter.h"
#include "STPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API ASTPlayerCharacter : public ASTBaseCharacter
{
	GENERATED_BODY()
public:
	ASTPlayerCharacter();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Camera Setup")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = "Camera Setup")
	TObjectPtr<UCameraComponent> FollowCamera;
};
