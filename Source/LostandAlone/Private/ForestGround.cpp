#include "ForestGround.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AForestGround::AForestGround()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create the Static Mesh Component and set it as the Root Component
	GroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundMesh"));
	RootComponent = GroundMesh;

	// Configure collision profile (adjust as needed)
	GroundMesh->SetCollisionProfileName(TEXT("BlockAll"));

	// Load a default static mesh (using the Engine's basic plane)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (MeshAsset.Succeeded())
	{
		GroundMesh->SetStaticMesh(MeshAsset.Object);
	}

	// Load the specified ground material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("Material'/Game/StarterContent/Materials/M_Ground_Grass.M_Ground_Grass'"));
	if (MaterialAsset.Succeeded())
	{
		GroundMesh->SetMaterial(0, MaterialAsset.Object);
	}
}

void AForestGround::BeginPlay()
{
	Super::BeginPlay();
	
}

void AForestGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

