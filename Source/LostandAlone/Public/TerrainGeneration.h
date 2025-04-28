#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainGeneration.generated.h"

UCLASS()
class LOSTANDALONE_API ATerrainGeneration : public AActor
{
	GENERATED_BODY()
	
public:	
	ATerrainGeneration();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
