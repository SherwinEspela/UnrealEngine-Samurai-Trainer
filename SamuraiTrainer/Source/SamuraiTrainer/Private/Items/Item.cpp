// // Copyright 2024 Sherwin Espela. All rights reserved.


#include "Items/Item.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
}

//void AItem::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//void AItem::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
