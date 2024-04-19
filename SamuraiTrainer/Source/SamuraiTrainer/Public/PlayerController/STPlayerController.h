// // Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STPlayerController.generated.h"

class ASTPlayerCharacter;
class UInputMappingContext;
class UInputAction;
class UEnhancedInputComponent;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class SAMURAITRAINER_API ASTPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SwordInteract();
	void Attack();

protected:
	// Player Inputs
	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* InputActionSwordInteract;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* InputActionAttack;

private:
	TObjectPtr<ASTPlayerCharacter> PlayerCharacter;
	UEnhancedInputComponent* EnhancedInputComponent;
	
};
