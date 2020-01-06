

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

	UWorld* world = GetWorld();

	// Get the player's viewpoint
	world->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerLocation, OUT playerRotation);

	FVector lineTraceDirection = playerLocation + (playerRotation.Vector() * reach);

	// Draw a red trace in the world to visualize
	DrawDebugLine(GetWorld(), playerLocation, lineTraceDirection, FColor(255, 0, 0), false, 0.0f, 0.0f, 10.0f);

	// Raycast out to reach distance
	FHitResult hit;
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());

	world->LineTraceSingleByObjectType(OUT hit, playerLocation, lineTraceDirection, ECollisionChannel::ECC_PhysicsBody, traceParameters);

	AActor* actorHit = hit.GetActor();
	if (actorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(actorHit->GetName()));
	}

	// See what we hit
}

