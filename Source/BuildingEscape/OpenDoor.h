

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void OpenDoor();
	void CloseDoor();

	UPROPERTY(VisibleAnywhere)
	float OpenAngle = -60.0f;

	UPROPERTY(VisibleAnywhere)
	float CloseAngle = 0.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens; // Pawn inherits from Actor

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.4f;

	float LastDoorOpenTime = 0.0f;
};
