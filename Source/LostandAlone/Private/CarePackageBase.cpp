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

	// 25 items
	int32 Code = FMath::RandRange(0, 25);

	switch (Code)
	{
		// Random Natural Stuff
	case 0: ItemName = "Rock"; break;
	case 1: ItemName = "Stick"; break;
	case 2: ItemName = "Fern"; break;
	case 3: ItemName = "Flower"; break;
	case 4: ItemName = "Sap"; break;
	case 5: ItemName = "Leaves"; break;
	case 6: ItemName = "Mushroom"; break;
	case 7: ItemName = "Still Water"; break;
	case 8: ItemName = "Wasp Nest"; break; // Hurts
	case 9: ItemName = "Snake"; break; // Hurts
		// Eatable
	case 10: ItemName = "Blueberry"; break;
	case 11: ItemName = "Redberry"; break;
	case 12: ItemName = "Greenberry"; break;
	case 13: ItemName = "Yellowberry"; break;
	case 14: ItemName = "Apple"; break;
	case 15: ItemName = "Rotten Apple"; break;
	case 16: ItemName = "Trail Mix"; break;
	case 17: ItemName = "Lunchly"; break;
	case 18: ItemName = "Deer Jerky"; break;
	case 19: ItemName = "Honeycomb"; break;
		// Tools (Only one of each)
	case 20: ItemName = "Flashlight"; break;
	case 21: ItemName = "Knife"; break;
	case 22: ItemName = "Spear"; break;
	case 23: ItemName = "Whistle"; break;
	case 24: ItemName = "Bow"; break;
	case 25: ItemName = "Horn"; break;
	}

	// Tools can only have 1
	if (Code >= 20) {
		ItemCount = 1;
	}
	// Everything else
	else {
		float Rand = FMath::FRand();
		// 60% for 1
		if (Rand < 0.6f) ItemCount = 1;
		// 30% for 2
		else if (Rand < 0.9f) ItemCount = 2;
		// 10% for 3
		else ItemCount = 3;
	}

	ContainsItem = true;
	
}

// Called every frame
void ACarePackageBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

