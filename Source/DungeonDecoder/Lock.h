// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trigger.h"
#include "Components/StaticMeshComponent.h"
#include "Lock.generated.h"

UCLASS()
class DUNGEONDECODER_API ALock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* KeyItemMesh;

	UPROPERTY(VisibleAnywhere)
	UTrigger* TriggerComp;

	UPROPERTY(EditAnywhere, Category = "Lock")
	FString KeyName;


	void SetIsKeyPlaced(bool NewKeyValue);
	bool GetIsKeyPlaced() const;


private:
	UPROPERTY(VisibleAnywhere)
	bool IsKeyPlaced = false;

};
