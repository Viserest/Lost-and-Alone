
#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryComponent::AddItem(FString Name, int32 Count)
{
	// If inventory already contains item then add to it
	if (int32* index = Items.Find(Name))
	{
		*index += Count;
	}
	// Add item to inventory with its count
	else {
		Items.Add(Name, Count);
	}
}

TMap<FString, int32> UInventoryComponent::GetItems()
{
	return Items;
}
