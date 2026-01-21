// Fill out your copyright notice in the Description page of Project Settings.


#include "Lock.h"

// Sets default values
ALock::ALock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("Lock"));


	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(Root);

	TriggerComp = CreateDefaultSubobject<UTrigger>(TEXT("Trigger Comp"));
	TriggerComp->SetupAttachment(Root);
	KeyItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key Item Mesh"));
	KeyItemMesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void ALock::BeginPlay()
{
	Super::BeginPlay();
	SetIsKeyPlaced(false);

}

// Called every frame
void ALock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALock::SetIsKeyPlaced(bool NewKeyValue)
{
	IsKeyPlaced = NewKeyValue;

	TriggerComp->Trigger(IsKeyPlaced);
	KeyItemMesh->SetVisibility(IsKeyPlaced);

	
}

bool ALock::GetIsKeyPlaced() const
{
	return IsKeyPlaced;

}

