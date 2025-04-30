// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarePackageBase.generated.h"

UCLASS()
class LOSTANDALONE_API ACarePackageBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACarePackageBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	int32 ItemCode;

	UPROPERTY()
	int32 ItemCount;

	UPROPERTY()
	bool ContainsItem;

};
