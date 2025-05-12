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

    static ConstructorHelpers::FObjectFinder<UBlueprint> TreeDeadBP(TEXT("Blueprint'/Game/Meshes/BP_TreeDead.BP_TreeDead'"));
    if (TreeDeadBP.Succeeded())
    {
        TreeDeadBPClass = (UClass*)TreeDeadBP.Object->GeneratedClass;
    }

    static ConstructorHelpers::FObjectFinder<UBlueprint> TreeSpruceBP(TEXT("Blueprint'/Game/Meshes/BP_TreeSpruce.BP_TreeSpruce'"));
    if (TreeSpruceBP.Succeeded())
    {
        TreeSpruceBPClass = (UClass*)TreeSpruceBP.Object->GeneratedClass;
    }

    static ConstructorHelpers::FObjectFinder<UBlueprint> BuildingBlueprint(TEXT("Blueprint'/Game/Meshes/BP_Building.BP_Building'"));
    if (BuildingBlueprint.Succeeded())
    {
        BuildingBPClass = (UClass*)BuildingBlueprint.Object->GeneratedClass;
    }

    static ConstructorHelpers::FObjectFinder<UBlueprint> CarePackageBlueprint(TEXT("Blueprint'/Game/Meshes/BP_CarePackage.BP_CarePackage'"));
    if (CarePackageBlueprint.Succeeded())
    {
        CarePackageBPClass = (UClass*)CarePackageBlueprint.Object->GeneratedClass;
    }

    static ConstructorHelpers::FObjectFinder<UBlueprint> BolderBlueprint(TEXT("Blueprint'/Game/Meshes/BP_Rock.BP_Rock'"));
    if (BolderBlueprint.Succeeded())
    {
        BolderBPClass = (UClass*)BolderBlueprint.Object->GeneratedClass;
    }
}

void ATerrainGeneration::BeginPlay()
{
	Super::BeginPlay();

    int32 Random = FMath::RandRange(0, 10000);
    TerrainSeed = FVector2D(Random, Random);

    Random = FMath::RandRange(0, 10000);
    BiomeSeed = FVector2D(Random, Random);

    GenerateTerrain(1000, 1000);
}

void ATerrainGeneration::GenerateTerrain(int32 SizeX, int32 SizeY)
{
    int32 TreeCount = 0;
    int32 BuildingCount = 0;
    int32 PackageCount = 0;
    int32 RockCount = 0;

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FColor> VertexColors;

    float Scale = 90.0f;
    float HeightScale = 20.0f;
    float PerlinMultiplier = 120.0f;

    // Move actor so 0,0 is middle of generated terrain
    SetActorLocation(FVector(-(SizeX / 2) * Scale, -(SizeY / 2) * Scale, 0));

    // Create grid of vertices
    for (int32 y = 0; y <= SizeY; y++)
    {
        int32 BuildingX = FMath::RandRange(0, SizeY);
        int32 CarePackageX = FMath::RandRange(0, SizeY);
        int32 BolderX = FMath::RandRange(0, SizeY);
        for (int32 x = 0; x <= SizeX; x++)
        {
            float XPos = x * Scale;
            float YPos = y * Scale;
            float ZPos;
            if (x == 0 || y == 0 || x == SizeX || y == SizeY) {
                ZPos = 1000.0f;
            }
            else {
                float Noise = FMath::PerlinNoise2D(FVector2D(x, y) / PerlinMultiplier + TerrainSeed);
                ZPos = Noise * HeightScale * PerlinMultiplier;
            }
            FVector Location = FVector(XPos, YPos, ZPos);
            FVector ActorLocation = FVector(XPos, YPos, ZPos) + GetActorLocation();
            Vertices.Add(Location);
            UVs.Add(FVector2D((float)x / SizeX, (float)y / SizeY));

            if (ZPos == 1000.0f) {
                VertexColors.Add(FColor::White);
            }
            else if (ZPos < -400.0f) {
                VertexColors.Add(FColor(0, 0, 50));
            }
            else {
                VertexColors.Add(FColor(0, 4, 0));

                float AddChance = FMath::FRand();

                // If current x equals the possible BuildingX
                if (x == BuildingX)
                {
                    // 5% chance of spawning
                    if (AddChance < 0.05f)
                    {
                        BuildingCount++;
                        GetWorld()->SpawnActor<AActor>(BuildingBPClass, ActorLocation, FRotator(0, 0, 0));
                    }
                }
                // If current x equals the possible CarePackageX
                else if (x == CarePackageX)
                {
                    // 40% chance of spawning
                    if (AddChance < 0.2f)
                    {
                        PackageCount++;
                        GetWorld()->SpawnActor<AActor>(CarePackageBPClass, ActorLocation, FRotator(0, 0, 0));
                    }
                }
                // If current x equals the possible BolderX
                else if (x == BolderX)
                {
                    // 40% chance of spawning
                    if (AddChance < 0.5f)
                    {
                        RockCount++;
                        GetWorld()->SpawnActor<AActor>(BolderBPClass, ActorLocation, FRotator(0, 0, 0));
                    }
                }
                // If nothing else can spawn then maybe place a tree
                else {
                    // 0.8% chance of spawning
                    if (AddChance < 0.008f)
                    {
                        TreeCount++;
                        float BiomeNoise = FMath::PerlinNoise2D(FVector2D(x, y) / PerlinMultiplier + BiomeSeed);
                        // From (-1 to -0.3)
                        if (BiomeNoise < -0.3f)
                        {
                            GetWorld()->SpawnActor<AActor>(TreeDeadBPClass, ActorLocation, FRotator(0, 0, 0));
                        }
                        // From [-0.3 to 0.3]
                        else if (BiomeNoise >= -0.3f && BiomeNoise <= 0.3f)
                        {
                            GetWorld()->SpawnActor<AActor>(TreeBlueprintClass, ActorLocation, FRotator(0, 0, 0));
                        }
                        // From (0.3 to 1)
                        else if (BiomeNoise > 0.3f)
                        {
                            GetWorld()->SpawnActor<AActor>(TreeSpruceBPClass, ActorLocation, FRotator(0, 0, 0));
                        }
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

    // Print the count of everything at end of generation
    UE_LOG(LogTemp, Warning, TEXT("World Size: %d | Trees: %d | Buildings: %d | Packages: %d | Rocks: %d"),
        SizeX, TreeCount, BuildingCount, PackageCount, RockCount);
}