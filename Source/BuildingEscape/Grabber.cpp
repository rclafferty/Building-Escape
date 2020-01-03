

#include "Grabber.h"

// Blank parameter for annotation
#define OUT 

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector playerLocation;
	FRotator playerRotation;

	// Get the player's viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerLocation, OUT playerRotation);

	FVector lineTraceDirection = playerLocation + (playerRotation.Vector() * reach);

	// Log viewpoint for testing
	// UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *playerLocation.ToString(), *playerRotation.ToString());

	// Draw a red trace in the world to visualize
	DrawDebugLine(GetWorld(), playerLocation, lineTraceDirection, FColor(255, 0, 0), false, 0.0f, 0.0f, 10.0f);

	// Raycast out to reach distance

	
	// See what we hit
}

