// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Combat/CombatSystemComponent.h"
#include "Character/STPlayerCharacter.h"
#include "Character/STEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

ACombatSystemComponent::ACombatSystemComponent()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ACombatSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Player = CastChecked<ASTPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	TSubclassOf<ASTEnemyCharacter> EnemyClass = ASTEnemyCharacter::StaticClass();
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, EnemyClass, OutActors);

	if (OutActors.Num() > 0)
	{
		for (auto FoundActor : OutActors) Enemies.Add(Cast<ASTEnemyCharacter>(FoundActor));
	}
}
