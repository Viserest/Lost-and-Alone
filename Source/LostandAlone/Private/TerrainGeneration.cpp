#include "TerrainGeneration.h"
#include "Kismet/KismetMathLibrary.h"

ATerrainGeneration::ATerrainGeneration()
{
	PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
    RootComponent = Mesh;

    ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Material/M_TerrainColor.M_TerrainColor'"));
    if (Material.Succeeded()) {
        TerrainMaterial = Material.Object;
    }

    static ConstructorHelpers::FObjectFinder<UBlueprint> TreeBlueprint(TEXT("Blueprint'/Game/Meshes/Tree.Tree'"));
    if (TreeBlueprint.Succeeded())
    {
        TreeBlueprintClass = (UClass*)TreeBlueprint.Object->GeneratedClass;
    }
}

void ATerrainGeneration::BeginPlay()
{
	Super::BeginPlay();

    int32 Random = FMath::RandRange(0, 10000);
    Seed = FVector2D(Random, Random);

    GenerateTerrain(400, 400);
}

void ATerrainGeneration::GenerateTerrain(int32 SizeX, int32 SizeY)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FColor> VertexColors;

    float Scale = 90.0f;
    float HeightScale = 20.0f;
    float HeightMultiplier = 120.0f;

    // Create grid of vertices
    for (int32 y = 0; y <= SizeY; y++)
    {
        for (int32 x = 0; x <= SizeX; x++)
        {
            float Noise = FMath::PerlinNoise2D(FVector2D(x, y) / HeightMultiplier + Seed);
            float XPos = x * Scale;
            float YPos = y * Scale;
            float ZPos = Noise * HeightScale * HeightMultiplier;
            FVector Location = FVector(XPos, YPos, ZPos);
            Vertices.Add(Location);
            UVs.Add(FVector2D((float)x / SizeX, (float)y / SizeY));

            // Color of ground based on height
            if (ZPos < -400.0f) {
                VertexColors.Add(FColor(0, 0, 50));
            }
            else {
                VertexColors.Add(FColor(0, 4, 0));

                // if tree should spawn (1% chance to spawn)
                float TreeChance = FMath::FRand();
                if (TreeChance < 0.01f) {
                    GetWorld()->SpawnActor<AActor>(TreeBlueprintClass, Location, FRotator(0, 0, 0));
                }
            }
        }
    }

    // Create triangles
    for (int32 y = 0; y < SizeY; y++)
    {
        for (int32 x = 0; x < SizeX; x++)
        {
            int32 i00 = x + y * (SizeX + 1);
            int32 i10 = (x + 1) + y * (SizeX + 1);
            int32 i01 = x + (y + 1) * (SizeX + 1);
            int32 i11 = (x + 1) + (y + 1) * (SizeX + 1);

            // First triangle
            Triangles.Append({ i00, i11, i10 });
            // Second triangle
            Triangles.Append({ i00, i01, i11 });
        }
    }

    // Let Unreal compute normals/tangents
    Mesh->CreateMeshSection(0, Vertices, Triangles, {}, UVs, VertexColors, {}, true);

    if (TerrainMaterial)
    {
        Mesh->SetMaterial(0, TerrainMaterial);
    }
}