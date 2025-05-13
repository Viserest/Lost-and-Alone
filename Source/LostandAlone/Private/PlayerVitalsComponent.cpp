
#include "PlayerVitalsComponent.h"
#include "Kismet/KismetMathLibrary.h"

UPlayerVitalsComponent::UPlayerVitalsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Max value of all vitals is 500
	MaxOfVital = 500.0f;
	
	// All vitals set to max
	Health = MaxOfVital;
	Hunger = MaxOfVital;
	Thirst = MaxOfVital;
	Energy = MaxOfVital;
	Comfort = MaxOfVital;
	Focus = MaxOfVital;
}

void UPlayerVitalsComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UPlayerVitalsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Once you spawn in, you will last 3m35s then die

	// Update vitals
	// All seconds until pain is this formula (max/rate=seconds)
	Hunger = FMath::Clamp(Hunger - 2.0f * DeltaTime, 0, MaxOfVital); // 250 seconds (4m10s) until pain
	Thirst = FMath::Clamp(Thirst - 3.0f * DeltaTime, 0, MaxOfVital); // 167 seconds (2m47s) until pain
	Energy = FMath::Clamp(Energy - 1.0f * DeltaTime, 0, MaxOfVital); // 500 seconds (8m20s) until pain
    Comfort = FMath::Clamp(Comfort - 2.0f * DeltaTime, 0, MaxOfVital);
    Focus = FMath::Clamp(Focus - 1.0f * DeltaTime, 0, MaxOfVital);

    /*
    Negative effects
    */
	// If hunger and thirst is completely empty
	if (Hunger <= 0.0f || Thirst <= 0.0f)
	{
		Health = FMath::Clamp(Health - 10.0f * DeltaTime, 0, MaxOfVital); // 50 seconds until death
	}
    // If your too comfortable then your focus will go down
    if (Comfort >= MaxOfVital * 0.8f)
    {
        Focus = FMath::Clamp(Focus - 3.0f * DeltaTime, 0, MaxOfVital);
    }
    /*
    Positive effects
    */
	// If hunger and thirst is 80% full add health
	if (Hunger >= MaxOfVital * 0.8f && Thirst >= MaxOfVital * 0.8f)
	{
		Health = FMath::Clamp(Health + 3.0f * DeltaTime, 0, MaxOfVital); // Add health until full
	}
	// If thirst is 70% full add focus
	if (Thirst >= MaxOfVital * 0.7f)
	{
		Focus = FMath::Clamp(Focus + 0.5f * DeltaTime, 0, MaxOfVital);
	}
}

// Player should only be able to eat 60-70 each meal
void UPlayerVitalsComponent::Eat(float Amount)
{
	Hunger = FMath::Clamp(Hunger + Amount, 0.0f, MaxOfVital);
	Energy = FMath::Clamp(Energy + Amount, 0.0f, MaxOfVital);
}

// Player should only be able to drink 30-60 each drink
void UPlayerVitalsComponent::Drink(float Amount)
{
	Thirst = FMath::Clamp(Thirst + Amount, 0.0f, MaxOfVital);
}

// Player should only be able to sleep 2-4 hours (Duration is by hour)
void UPlayerVitalsComponent::Sleep(float Duration)
{
	Energy = FMath::Clamp(Energy + Duration * 12.0f, 0.0f, MaxOfVital);
	Focus = FMath::Clamp(Focus + Duration * 5.0f, 0.0f, MaxOfVital);
	Comfort = FMath::Clamp(Comfort + Duration * 8.0f, 0.0f, MaxOfVital);
}

// Player should only lose 10-20 health per hit
void UPlayerVitalsComponent::TakeDamage(float Amount)
{
	Health = FMath::Clamp(Health - Amount, 0.0f, MaxOfVital);
}

