#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/PostProcessComponent.h"
#include "ProceduralNightLighting.generated.h"

UCLASS()
class LOSTANDALONE_API AProceduralNightLighting : public AActor
{
    GENERATED_BODY()

public:
    AProceduralNightLighting();

protected:
    virtual void BeginPlay() override;

private:
    /** Moonlight directional light */
    UPROPERTY(VisibleAnywhere, Category = "Night Lighting")
    UDirectionalLightComponent* MoonLight;

    /** Subtle ambient skylight */
    UPROPERTY(VisibleAnywhere, Category = "Night Lighting")
    USkyLightComponent* NightSkyLight;

    /** Post-process component for subtle color grading */
    UPROPERTY(VisibleAnywhere, Category = "Night Lighting")
    UPostProcessComponent* PostProcess;
};