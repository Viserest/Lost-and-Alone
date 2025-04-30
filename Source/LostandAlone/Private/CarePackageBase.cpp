// Fill out your copyright notice in the Description page of Project Settings.


#include "CarePackageBase.h"

// Sets default values
ACarePackageBase::ACarePackageBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACarePackageBase::BeginPlay()
{
	Super::BeginPlay();

	ItemCode = FMath::RandRange(0, 20);
	ItemCount = FMath::RandRange(1, 3);

	ContainsItem = true;
	
}

// Called every frame
void ACarePackageBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

