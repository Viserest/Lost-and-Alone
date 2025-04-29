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

    static ConstructorHelpers::FObjectFinder<UBlueprint> TreeBlueprint(TEXT("Blueprint'/Game/Meshes/BP_Tree.BP_Tree'"));
    if (TreeBlueprint.Succeeded())
    {
        TreeBlueprintClass = (UClass*)TreeBlueprint.Object->GeneratedClass;
    }

    static ConstructorHelpers::FObjectFinder<UBlueprint> BuildingBlueprint(TEXT("Blueprint'/Game/Meshes/BP_Building.BP_Building'"));
    if (BuildingBlueprint.Succeeded())
    {
        BuildingBPClass = (UClass*)BuildingBlueprint.Object->GeneratedClass;
    }
}

void ATerrainGeneration::BeginPlay()
{
	Super::BeginPlay();

    int32 Random = FMath::RandRange(0, 10000);
    TerrainSeed = FVector2D(Random, Random);

    Random = FMath::RandRange(0, 10000);
    SpawnSeed = FVector2D(Random, Random);

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
    float PerlinMultiplier = 120.0f;

    // Create grid of vertices
    for (int32 y = 0; y <= SizeY; y++)
    {
        for (int32 x = 0; x <= SizeX; x++)
        {
            float Noise = FMath::PerlinNoise2D(FVector2D(x, y) / PerlinMultiplier + TerrainSeed);
            float XPos = x * Scale;
            float YPos = y * Scale;
            float ZPos = Noise * HeightScale * PerlinMultiplier;
            FVector Location = FVector(XPos, YPos, ZPos);
            Vertices.Add(Location);
            UVs.Add(FVector2D((float)x / SizeX, (float)y / SizeY));

            // Color of ground based on height
            float BiomeNoise = FMath::PerlinNoise2D(FVector2D(x, y) / PerlinMultiplier + SpawnSeed);

            if (ZPos < -400.0f) {
                VertexColors.Add(FColor(0, 0, 50));
            }
            else {
                VertexColors.Add(FColor(0, 4, 0));

                float AddChance = FMath::FRand();

                // if tree should spawn
                // Noise between (-1, 0.6]
                if (BiomeNoise > -1.0f && BiomeNoise <= 0.4f)
                {
                    // 1% chance of spawning
                    if (AddChance < 0.01f)
                    {
                        GetWorld()->SpawnActor<AActor>(TreeBlueprintClass, Location, FRotator(0, 0, 0));
                    }
                }
                // if building should spawn
                // Noise between [0.8, 1)
                else if (BiomeNoise < 1.0f && BiomeNoise >= 0.6f)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Building could spawned in"));
                    // 10% chance of spawning
                    if (AddChance < 0.05f)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Building spawned"));
                        GetWorld()->SpawnActor<AActor>(BuildingBPClass, Location, FRotator(0, 0, 0));
                    }
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