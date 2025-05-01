#include "ProceduralNightLighting.h"
#include "Engine/World.h"

AProceduralNightLighting::AProceduralNightLighting()
{
    PrimaryActorTick.bCanEverTick = false;

    // Root component
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    // Moonlight (soft blue directional light)
    MoonLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("MoonLight"));
    MoonLight->SetupAttachment(RootComponent);
    MoonLight->SetMobility(EComponentMobility::Movable);
    MoonLight->Intensity = 30.0f;
    MoonLight->LightColor = FColor(180, 200, 255);
    MoonLight->SetCastShadows(true);
    MoonLight->ShadowBias = 0.5f;
    MoonLight->SetWorldRotation(FRotator(-30.f, -45.f, 0.f));

    // Night sky ambient
    NightSkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("NightSkyLight"));
    NightSkyLight->SetupAttachment(RootComponent);
    NightSkyLight->SetMobility(EComponentMobility::Movable);
    NightSkyLight->Intensity = 10.0f;
    NightSkyLight->LightColor = FColor(100, 100, 150);
    NightSkyLight->SourceType = ESkyLightSourceType::SLS_CapturedScene;

    // Post-process for subtle blue tint
    PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("NightPostProcess"));
    PostProcess->SetupAttachment(RootComponent);
    PostProcess->bUnbound = true;
    PostProcess->Settings.bOverride_ColorSaturation = true;
    PostProcess->Settings.ColorSaturation = FVector4(0.8f, 0.9f, 1.1f, 1.0f);
}

void AProceduralNightLighting::BeginPlay()
{
    Super::BeginPlay();

    // Re-capture sky for ambient
    NightSkyLight->RecaptureSky();
}