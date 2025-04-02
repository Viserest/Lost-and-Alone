#include "NightSky.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture2D.h"

ANightSky::ANightSky()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create the SkySphere component and set it as the root component
    SkySphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkySphere"));
    RootComponent = SkySphere;

    // Load the SkySphere mesh from the engine
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SkyMesh(TEXT("StaticMesh'/Engine/EngineSky/SM_SkySphere.SM_SkySphere'"));
    if (SkyMesh.Succeeded())
    {
        SkySphere->SetStaticMesh(SkyMesh.Object);
        // Scale up the sky sphere to cover your scene
        SkySphere->SetRelativeScale3D(FVector(100.0f));
    }

    // Load the star texture using ConstructorHelpers (allowed in the constructor)
    static ConstructorHelpers::FObjectFinder<UTexture2D> StarsTexFinder(TEXT("Texture2D'/Engine/EngineSky/T_Sky_Stars.T_Sky_Stars'"));
    if (StarsTexFinder.Succeeded())
    {
        StarsTexture = StarsTexFinder.Object;
    }
    else
    {
        StarsTexture = nullptr;
    }

    // Create the Moon component
    Moon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Moon"));
    Moon->SetupAttachment(RootComponent);

    // Load the moon sphere mesh from StarterContent
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MoonMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    if (MoonMesh.Succeeded())
    {
        Moon->SetStaticMesh(MoonMesh.Object);
        // Scale the moon to a smaller size
        Moon->SetRelativeScale3D(FVector(0.5f));
    }

    // Position the moon in the sky (adjust as needed)
    Moon->SetRelativeLocation(FVector(2000.f, 0.f, 1500.f));

    // Create a directional light component for moonlight
    MoonLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("MoonLight"));
    MoonLight->SetupAttachment(RootComponent);
    // Rotate the light to simulate moonlight coming from the moon's direction
    MoonLight->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
    MoonLight->Intensity = 1.0f; // Adjust intensity as needed
    MoonLight->LightColor = FColor(200, 200, 255); // Soft bluish tint
}

void ANightSky::BeginPlay()
{
    Super::BeginPlay();

    // Get the material from the SkySphere
    UMaterialInterface* BaseMat = SkySphere->GetMaterial(0);
    if (BaseMat)
    {
        // Create a dynamic material instance to override parameters at runtime
        UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(BaseMat, this);

        // Use the texture loaded in the constructor to set the star parameters
        if (StarsTexture)
        {
            DynMat->SetTextureParameterValue("StarsTexture1", StarsTexture);
            DynMat->SetTextureParameterValue("StarsTexture2", StarsTexture);
        }

        // Assign the dynamic material instance back to the SkySphere
        SkySphere->SetMaterial(0, DynMat);
    }
}

void ANightSky::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}