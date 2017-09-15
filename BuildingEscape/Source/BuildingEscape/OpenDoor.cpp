// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool PlayerEnteredRoom;

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Door = GetOwner();
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	// Used for one time closing door when player enters the room
	// It loops infinitely if not used
	PlayerEnteredRoom = false; 

	OpenTheDoor();
}

void UOpenDoor::OpenTheDoor()
{
	// Used in the TickComponent function below
	CurrentDoorAction = DoorAction::OPEN;
	
}

void UOpenDoor::CloseTheDoor()
{
	// Used in the TickComponent function below
	CurrentDoorAction = DoorAction::CLOSE;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume
	// Open door when pawn is on trigger volume
	// TODO Needed for completing game
	// ===========================================================
	if (PressurePlateObjective->IsOverlappingActor(PlayerPawn)) {
		OpenTheDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	// ===========================================================

	// Poll the other trigger volume
	// Lock the player in the room upon entering
	// DO THIS ONCE ONLY !
	if (PressurePlateLockPlayer->IsOverlappingActor(PlayerPawn) && !PlayerEnteredRoom) {
		CloseTheDoor();
		PlayerEnteredRoom = true; // !
	}

	// Dynamically open the door
	if (CurrentDoorAction == DoorAction::OPEN)
	{
		float CurrentAngle = Door->GetActorRotation().Yaw;

		// Keep opening as long as current yaw (<0 degrees) is lower than opened yaw (0 degrees)
		if (CurrentAngle < OpenAngle)
		{
			Door->SetActorRotation(FRotator(0.0f, CurrentAngle + DoorOpenSpeed, 0.0f));
		}
		// If it is fully opened then set the door action to null
		else
		{
			// Set door yaw to exactly OpenAngle (for esthetic purposes)
			Door->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
			CurrentDoorAction = DoorAction::NO_ACTION;
		}
	}
	// Dynamically close the door
	else if (CurrentDoorAction == DoorAction::CLOSE)
	{
		float CurrentAngle = Door->GetActorRotation().Yaw;
		
		// Keep closing as long as current yaw (>-90 degrees) is greater than closed yaw (-90 degrees)
		if (CurrentAngle > ClosedAngle)
		{
			Door->SetActorRotation(FRotator(0.0f, CurrentAngle + DoorCloseSpeed, 0.0f));
		}
		// If it is fully closed then set the door action to null
		else
		{
			// Set door yaw to exactly ClosedAngle (for esthetic purposes)
			Door->SetActorRotation(FRotator(0.0f, ClosedAngle, 0.0f));
			CurrentDoorAction = DoorAction::NO_ACTION;
		}
	}
}

