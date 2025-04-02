#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NightSky.generated.h"

UCLASS()
class LOSTANDALONE_API ANightSky : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ANightSky();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Sky sphere component for the night sky
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NightSky")
    class UStaticMeshComponent* SkySphere;

    // Moon mesh component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NightSky")
    class UStaticMeshComponent* Moon;

    // Directional light component to simulate moonlight
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NightSky")
    class UDirectionalLightComponent* MoonLight;

private:
    // Pointer to the star texture loaded in the constructor
    UTexture2D* StarsTexture;
};