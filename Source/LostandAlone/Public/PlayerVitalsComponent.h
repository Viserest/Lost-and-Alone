// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerVitalsComponent.generated.h"

UCLASS()
class LOSTANDALONE_API UPlayerVitalsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerVitalsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void Eat(float Amount);
    void Drink(float Amount);
    void Sleep(float Duration);
    void TakeDamage(float Amount);

    // Stats
    UPROPERTY()
    float MaxOfVital;

    UPROPERTY(EditAnywhere, Category = "Vitals")
    float Health;
    UPROPERTY(EditAnywhere, Category = "Vitals")
    float Hunger;
    UPROPERTY(EditAnywhere, Category = "Vitals")
    float Thirst;
    UPROPERTY(EditAnywhere, Category = "Vitals")
    float Energy;
    UPROPERTY(EditAnywhere, Category = "Vitals")
    float Comfort;
    UPROPERTY(EditAnywhere, Category = "Vitals")
    float Focus;
};
