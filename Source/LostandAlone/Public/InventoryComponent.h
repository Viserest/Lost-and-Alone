
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

USTRUCT()
struct FItem {
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	int32 Count;

	bool operator==(const FItem& Other) const
	{
		return Name == Other.Name;
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOSTANDALONE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddItem(FString Name, int32 Count);
	void RemoveItem(FItem& Item);
	TArray<FItem> GetItems();
	FItem& GetSelected();

	int32 SelectedItem = 0;

private:

	UPROPERTY()
	TArray<FItem> Items;
};
