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
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* Mesh;

	void GenerateTerrain(int32 SizeX, int32 SizeY, float Scale, float HeightScale);
};
