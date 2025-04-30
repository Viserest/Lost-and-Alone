#include "LAPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerVitalsComponent.h"
#include "InventoryComponent.h"
#include "CarePackageBase.h"

ALAPlayer::ALAPlayer()
{
	// Set size for collision capsule (default provided by ACharacter)
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Set base turn and look up rates
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a First Person Camera Component
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCameraComponent->SetupAttachment(GetCapsuleComponent());
	// Position the camera; adjust the Z offset to match your character’s eye level
	FPSCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 64.f));
	// Allow the camera to rotate based on the controller input
	FPSCameraComponent->bUsePawnControlRotation = true;

	// Play audio which is attached to player
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	// Get Splashing sound effect
	ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset(TEXT("SoundWave'/Game/Audio/water_sound.water_sound'"));
	if (SoundAsset.Succeeded())
	{
		SplashSound = SoundAsset.Object;
	}

	// Player vitals for health, hunger, thirst, drousiness, energy, fear
	VitalsComponent = CreateDefaultSubobject<UPlayerVitalsComponent>(TEXT("VitalsComponent"));

	// Player inventory
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void ALAPlayer::BeginPlay()
{
	Super::BeginPlay();

	// When player hits object run this function
	OnActorBeginOverlap.AddDynamic(this, &ALAPlayer::OnOverlapBegin);

	// Allow flying
	//GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	
}

void ALAPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString StatsText = FString::Printf(TEXT("Health: %.0f | Hunger: %.0f | Thirst: %.0f | Energy: %.0f"),
		VitalsComponent->Health,
		VitalsComponent->Hunger,
		VitalsComponent->Thirst,
		VitalsComponent->Energy
	);

	GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Green, StatsText);

}

void ALAPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ALAPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALAPlayer::MoveRight);

	// Bind movement jump
	PlayerInputComponent->BindAxis("MoveUp", this, &ALAPlayer::MoveUp);

	// Bind mouse look events (direct input)
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Optionally, bind rate-based input for consistent rotation speed
	PlayerInputComponent->BindAxis("TurnRate", this, &ALAPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALAPlayer::LookUpAtRate);
}

void ALAPlayer::MoveForward(float Value)
{
	if (Controller && Value != 0.0f)
	{
		// Get the controller's rotation and form a forward direction ignoring pitch
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Calculate the forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// If Player is in water
		FVector PlayerLocation = GetActorLocation();
		if (PlayerLocation.Z < -380.0f) {
			Value *= 0.5;

			if (!AudioComponent->IsPlaying()) {
				AudioComponent->SetSound(SplashSound);
				AudioComponent->Play();
			}
		}
		else if (AudioComponent->IsPlaying()) {
			AudioComponent->Stop();
		}

		AddMovementInput(Direction, Value);
	}
}

void ALAPlayer::MoveRight(float Value)
{
	if (Controller && Value != 0.0f)
	{
		// Get the controller's rotation and form a right direction ignoring pitch
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Calculate the right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// If Player is in water
		FVector PlayerLocation = GetActorLocation();
		if (PlayerLocation.Z < -380.0f) {
			Value *= 0.5;

			if (!AudioComponent->IsPlaying()) {
				AudioComponent->SetSound(SplashSound);
				AudioComponent->Play();
			}
		}
		else if (AudioComponent->IsPlaying()) {
			AudioComponent->Stop();
		}

		AddMovementInput(Direction, Value);
	}
}

void ALAPlayer::MoveUp(float Value)
{
	// Begin upward movement
	if (Value != 0.0f)
	{
		AddMovementInput(FVector(0, 0, 1), Value);
	}
}

void ALAPlayer::TurnAtRate(float Rate)
{
	// Calculate delta for this frame from the rate information and apply yaw input
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALAPlayer::LookUpAtRate(float Rate)
{
	// Calculate delta for this frame from the rate information and apply pitch input
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ALAPlayer::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor->ActorHasTag("CarePackage"))
	{
		ACarePackageBase* CarePackage = Cast<ACarePackageBase>(OtherActor);
		if (CarePackage && CarePackage->ContainsItem)
		{
			int32 Code = CarePackage->ItemCode;
			int32 Count = CarePackage->ItemCount;

			CarePackage->ContainsItem = false;

			UE_LOG(LogTemp, Warning, TEXT("Care Package -> Code: %d, Count: %d"), Code, Count);
		}
	}
}