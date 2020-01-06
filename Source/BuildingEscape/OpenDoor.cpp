#include "OpenDoor.h"

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

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
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

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	// If ActorThatOpens is in the volume
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
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