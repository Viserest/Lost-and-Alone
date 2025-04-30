#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "TerrainGeneration.generated.h"

UCLASS()
class LOSTANDALONE_API ATerrainGeneration : public AActor
{
	GENERATED_BODY()
	
public:
	ATerrainGeneration();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FVector2D TerrainSeed = FVector2D::ZeroVector;

	UPROPERTY()
	FVector2D SpawnSeed = FVector2D::ZeroVector;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* Mesh = nullptr;

	UPROPERTY()
	UMaterial* TerrainMaterial = nullptr;

	UPROPERTY()
	UClass* TreeBlueprintClass = nullptr;

	UPROPERTY()
	UClass* BuildingBPClass = nullptr;

	UPROPERTY()
	UClass* CarePackageBPClass = nullptr;

	UPROPERTY()
	UClass* BolderBPClass = nullptr;

	void GenerateTerrain(int32 SizeX, int32 SizeY);
};
