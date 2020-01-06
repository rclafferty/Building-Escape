

#include "Grabber.h"

// Blank parameter for annotation
#define OUT 

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		// input component is found
		UE_LOG(LogTemp, Warning, TEXT("Input component is found"));

		// Bind the input action
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component is not found on object %s"), *(GetOwner()->GetName()));
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	// Look for attached physics handle
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!physicsHandle)
	{
		// Error -- Not found
		UE_LOG(LogTemp, Error, TEXT("Physics handle is not found on object %s"), *(GetOwner()->GetName()));
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If the physics handle is attached
	if (physicsHandle->GrabbedComponent)
	{
		FVector playerLocation;
		FRotator playerRotation;

		// Get the player's viewpoint
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerLocation, OUT playerRotation);

		FVector lineTraceDirection = playerLocation + (playerRotation.Vector() * reach);

		// Move the object we're holding
		physicsHandle->SetTargetLocation(playerLocation);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab action pressed"));

	// Line trace and see if we reach any actors with physics body collision channel set
	FHitResult hit = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* componentToGrab = hit.GetComponent();
	AActor* actorHit = hit.GetActor();

	// If we hit something
	if (actorHit)
	{
		// attach a physics handle
		physicsHandle->GrabComponent(componentToGrab, NAME_None, componentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Object was released"));
	// TODO: Release physics handle
	physicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	UWorld* world = GetWorld();

	FVector playerLocation;
	FRotator playerRotation;

	// Get the player's viewpoint
	world->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerLocation, OUT playerRotation);

	FVector lineTraceDirection = playerLocation + (playerRotation.Vector() * reach);

	// Debug: Draw a red trace in the world to visualize
	// DrawDebugLine(GetWorld(), playerLocation, lineTraceDirection, FColor(255, 0, 0), false, 0.0f, 0.0f, 10.0f);

	// Raycast out to reach distance
	FHitResult hit;
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());

	world->LineTraceSingleByObjectType(OUT hit, playerLocation, lineTraceDirection, ECollisionChannel::ECC_PhysicsBody, traceParameters);

	// See what we hit
	AActor* actorHit = hit.GetActor();
	if (actorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(actorHit->GetName()));
	}

	return hit;
}
