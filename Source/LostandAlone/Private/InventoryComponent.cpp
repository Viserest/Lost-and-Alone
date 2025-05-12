
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

struct Item {
	FString Name;
	int32 Count;
};

void UInventoryComponent::AddItem(FString Name, int32 Count)
{
	bool bFound = false;
	for (FItem& CurrentItem : Items)
	{
		// If inventory already contains item then add to it
		if (CurrentItem.Name == Name)
		{
			CurrentItem.Count += Count;
			bFound = true;
			break;
		}
	}
	// Add item to inventory with its count
	if (!bFound)
	{
		Items.Add(FItem(Name, Count));
	}
}

TArray<FItem> UInventoryComponent::GetItems()
{
	return Items;
}

FString UInventoryComponent::GetSelected()
{
	return Items[SelectedItem].Name;
}
