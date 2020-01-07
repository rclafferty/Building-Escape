#include "Grabber.h"

#define NULLPTR_LOG(ptr) UE_LOG(LogTemp, Error, TEXT("%s was a nullptr on %s"), TEXT(#ptr), *GetOwner()->GetName())

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
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		// input component is found
		UE_LOG(LogTemp, Warning, TEXT("Input component is found"));

		// Bind the input action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		// UE_LOG(LogTemp, Error, TEXT("Input component is not found on object %s"), *(GetOwner()->GetName()));
		NULLPTR_LOG(InputComponent);
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		// Error -- Not found
		// UE_LOG(LogTemp, Error, TEXT("Physics handle is not found on object %s"), *(GetOwner()->GetName()));
		NULLPTR_LOG(PhysicsHandle);
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle)
	{
		// If the physics handle is attached
		if (PhysicsHandle->GrabbedComponent)
		{
			// Move the object we're holding
			PhysicsHandle->SetTargetLocation(GetReachLineEnd());
		}
	}
	else
	{
		NULLPTR_LOG(PhysicsHandle);
	}
}

void UGrabber::Grab()
{
	if (PhysicsHandle)
	{
		// Line trace and see if we reach any actors with physics body collision channel set
		FHitResult PhysicsBodyHitResult = GetFirstPhysicsBodyInReach();
		UPrimitiveComponent* ComponentToGrab = PhysicsBodyHitResult.GetComponent();

		if (ComponentToGrab)
		{
			AActor* ActorHit = PhysicsBodyHitResult.GetActor();

			// If we hit something
			if (ActorHit)
			{
				// attach a physics handle
				PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true); // hit component, no bones needed, grab at anchor point, allow rotation
			}
			else
			{
				NULLPTR_LOG(ActorHit);
			}
		}
		else
		{
			NULLPTR_LOG(ComponentToGrab);
		}

		UE_LOG(LogTemp, Warning, TEXT("Grab action pressed"));
	}
}

void UGrabber::Release()
{
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Object was released"));
		PhysicsHandle->ReleaseComponent();
	}
	else
	{
		NULLPTR_LOG(PhysicsHandle);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Raycast out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, GetReachLineStart(), GetReachLineEnd(), ECollisionChannel::ECC_PhysicsBody, traceParameters);

	return HitResult;
}

FVector UGrabber::GetReachLineStart() const
{
	FVector PlayerLocation;
	FRotator PlayerRotation;

	// Get the player's viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);

	return PlayerLocation;
}

FVector UGrabber::GetReachLineEnd() const
{
	FVector PlayerLocation;
	FRotator PlayerRotation;

	// Get the player's viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);

	return (PlayerLocation + (PlayerRotation.Vector() * Reach));
}
