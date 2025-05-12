#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "PlayerVitalsComponent.h"
#include "InventoryComponent.h"
#include "LAPlayer.generated.h"

UCLASS()
class LOSTANDALONE_API ALAPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	
	ALAPlayer();

private:
	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* SplashSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* WhistleSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* HornSound = nullptr;

	UPROPERTY()
	UPlayerVitalsComponent* VitalsComponent = nullptr;

	UPROPERTY()
	UInventoryComponent* InventoryComponent = nullptr;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Movement functions
	void MoveForward(float Value);
	void MoveRight(float Value);

	// Jump function
	void MoveUp(float Value);

	// Look functions based on input rates
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// Interact
	void Interact();
	void ScrollInventory(float Value);

protected:

	// First Person Camera Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FPSCameraComponent;

	// Base turn rate, in deg/sec.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	// Base look up/down rate, in deg/sec.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;
};
