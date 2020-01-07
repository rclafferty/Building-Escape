#include "OpenDoor.h"

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
	if (GetTotalMassOfActorsOnPlate() > 30.0f) // TODO: Make into a parameter
	{
		// Open the door
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

void UOpenDoor::OpenDoor()
{
	// Create a rotator
	FRotator newRotation = FRotator(0.0f, OpenAngle, 0.0f);

	// Set the door rotation
	GetOwner()->SetActorRelativeRotation(newRotation);
}

void UOpenDoor::CloseDoor()
{
	// Create a rotator
	FRotator NewRotation = FRotator(0.0f, CloseAngle, 0.0f);

	// Set the door rotation
	GetOwner()->SetActorRelativeRotation(NewRotation);
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() const
{
	float TotalMass = 0.0f;

	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);

	// Iterate through them adding their masses
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		// UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *(Actor->GetName()));
	}

	// UE_LOG(LogTemp, Warning, TEXT("Total mass on pressure plate: %f"), (TotalMass));
	return TotalMass;
}