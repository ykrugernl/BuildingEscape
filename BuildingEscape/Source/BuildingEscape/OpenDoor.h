// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenTheDoor();
	void CloseTheDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	AActor* PlayerPawn;
	AActor* Door;

	// These variables can be viewed in Unreal Editor
	UPROPERTY(VisibleAnywhere)
		float OpenAngle = 0.0f;
	UPROPERTY(VisibleAnywhere)
		float ClosedAngle = -90.0f;

	// These variables can be edited in Unreal Editor
	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlateObjective;
	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlateLockPlayer;
	UPROPERTY(EditAnywhere)
		float DoorDelay = 0.8f;
	UPROPERTY(EditAnywhere)
		float DoorCloseSpeed = -1.0f;
	UPROPERTY(EditAnywhere)
		float DoorOpenSpeed = 0.5f;

	// Used in TickComponent
	enum DoorAction { NO_ACTION, OPEN, CLOSE };
	DoorAction CurrentDoorAction = DoorAction::NO_ACTION;
	float LastDoorOpenTime;
};
