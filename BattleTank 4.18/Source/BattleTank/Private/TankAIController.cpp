// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	auto PlayerTank = GetPlayerTank();
	if (!PlayerTank) { return; }
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController found: %s"), *(PlayerTank->GetName()));
	}
	//t
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetPlayerTank())
	{
		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
	}
}

ATank* ATankAIController::GetPlayerTank() const
{
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) { return nullptr; }
	return Cast<ATank>(PlayerPawn);
}
