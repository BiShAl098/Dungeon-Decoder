// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger.h"

UTrigger::UTrigger()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (ActorToMove && (Mover = ActorToMove->FindComponentByClass<UMove>())) {
		UE_LOG(LogTemp, Display, TEXT("Current Actor to Move: %s"), *ActorToMove->GetActorNameOrLabel());

	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No Actor assigned to Trigger or Actor has no Move component."));
	}
	if(isPressurePlate){
	OnComponentBeginOverlap.AddDynamic(this, &UTrigger::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UTrigger::OnEndOverlap);
	}
}



void UTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UTrigger::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,	const FHitResult& SweepResult) 
{

	
	if (OtherActor && OtherActor->ActorHasTag(FName("Activator")))
	{
		OverlappingActors++;

		if (Mover) {
			Trigger(true);
		}

	}
	
}


void UTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && OtherActor->ActorHasTag(FName("Activator")))
	{
		OverlappingActors--;

		if (Mover && !OverlappingActors) {
			Trigger(false);
		}

	}
}

void UTrigger::Trigger(bool NewTriggerValue) {
	
	isTriggered = NewTriggerValue;

	if (Mover) {
		Mover->SetShouldMove(isTriggered);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No Actor assigned to Mover or has no Move component."));
	}
}
