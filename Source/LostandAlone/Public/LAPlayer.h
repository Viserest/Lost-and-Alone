#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LAPlayer.generated.h"

UCLASS()
class LOSTANDALONE_API ALAPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	
	ALAPlayer();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Movement functions
	void MoveForward(float Value);
	void MoveRight(float Value);

	// Look functions based on input rates
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

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
