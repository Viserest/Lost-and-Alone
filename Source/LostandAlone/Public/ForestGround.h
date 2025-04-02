#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForestGround.generated.h"

UCLASS()
class LOSTANDALONE_API AForestGround : public AActor
{
	GENERATED_BODY()
	
public:

	AForestGround();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	// Static Mesh Component for the forest ground
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Forest")
	class UStaticMeshComponent* GroundMesh;
};
