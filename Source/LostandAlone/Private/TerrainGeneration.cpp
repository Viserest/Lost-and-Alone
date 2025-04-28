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

    GenerateTerrain();
	
}

void ATerrainGeneration::GenerateTerrain() {
    // vertice components
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FLinearColor> VertexColors;
    TArray<FProcMeshTangent> Tangents;

    int32 Width = 100;
    int32 Depth = 100;
    float Scale = 20.0f;
    float HeightMultiplier = 100.0f;

    // loop over width and depth to generate height
    for (int32 x = 0; x <= Width; x++) {
        for (int32 y = 0; y <= Depth; y++) {
            // Generate height from perlin noise
            float Height = FMath::PerlinNoise2D(FVector2D(x, y) / Scale) * HeightMultiplier;
            Vertices.Add(FVector(x * 100, y * 100, Height));
            UVs.Add(FVector2D((float)x / Width, (float)y / Depth));
        }
    }

    // Add each triangle section
    for (int32 x = 0; x < Width; x++) {
        for (int32 y = 0; y < Depth; y++) {
            int32 Index = x * (Depth + 1) + y;

            // Creating a square so two triangle sections are needed
            Triangles.Add(Index);
            Triangles.Add(Index + Depth + 1);
            Triangles.Add(Index + 1);

            Triangles.Add(Index + 1);
            Triangles.Add(Index + Depth + 1);
            Triangles.Add(Index + Depth + 2);
        }
    }

    // Add section to procedural mesh
    Mesh->CreateMeshSection_LinearColor(
        0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true
    );
}
