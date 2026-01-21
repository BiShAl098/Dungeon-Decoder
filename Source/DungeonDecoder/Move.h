// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Move.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONDECODER_API UMove : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMove();

	void SetShouldMove(bool bNewShouldMove);

	bool GetShouldMove();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	FVector StartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	FVector	MoveOffset;

	FVector	TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	float MoveTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")

	bool ReachedTarget;

private:
	bool ShouldMove;

		
};
