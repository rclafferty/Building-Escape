#include "OpenDoor.h"

#define NULLPTR_LOG(ptr) UE_LOG(LogTemp, Error, TEXT("%s was a nullptr on %s"), TEXT(#ptr), *GetOwner()->GetName())

// Blank parameter for annotation
#define OUT 

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) // TODO: Make into a parameter
	{
		// Open the door
		// Broadcast event to blueprint
		OnOpen.Broadcast();
	}
	// Close the door
	else
	{
		// Broadcast event to blueprint
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() const
{
	float TotalMass = 0.0f;

	if (PressurePlate)
	{
		// Find all the overlapping actors
		TArray<AActor*> OverlappingActors;
		PressurePlate->GetOverlappingActors(OverlappingActors);

		// Iterate through them adding their masses
		for (const auto* Actor : OverlappingActors)
		{
			TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
			// UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *(Actor->GetName()));
		}
	}

	// UE_LOG(LogTemp, Warning, TEXT("Total mass on pressure plate: %f"), (TotalMass));
	return TotalMass;
}