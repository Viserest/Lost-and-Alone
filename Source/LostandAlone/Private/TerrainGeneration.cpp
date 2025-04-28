#include "TerrainGeneration.h"
#include "Kismet/KismetMathLibrary.h"

ATerrainGeneration::ATerrainGeneration()
{
	PrimaryActorTick.bCanEverTick = false;
    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
    RootComponent = Mesh;
}

void ATerrainGeneration::BeginPlay()
{
	Super::BeginPlay();
    GenerateTerrain(128, 128, 100.0f, 50.0f);
}

void ATerrainGeneration::GenerateTerrain(int32 SizeX, int32 SizeY, float Scale, float HeightScale)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;

    // Create grid of vertices
    for (int32 y = 0; y <= SizeY; y++)
    {
        for (int32 x = 0; x <= SizeX; x++)
        {
            float XPos = x * Scale;
            float YPos = y * Scale;
            float Noise = FMath::PerlinNoise2D(FVector2D(x, y) * 0.1f);
            float ZPos = Noise * HeightScale;
            Vertices.Add(FVector(XPos, YPos, ZPos));
            UVs.Add(FVector2D((float)x / SizeX, (float)y / SizeY));
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
    Mesh->CreateMeshSection(0, Vertices, Triangles, {}, UVs, {}, {}, true);
}