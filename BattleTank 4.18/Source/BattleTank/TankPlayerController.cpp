// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) { return; }
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController possessing: %s"), *(ControlledTank->GetName()));
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	///Aim towards crosshair();
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }
	FVector HitLocation; // Out parameter;
	if (GetSightRayHitLocation(HitLocation)) //has "side-effect", is going to line trace.
	{
		GetControlledTank()->AimAt(HitLocation);
	}
}

//get world location of linetrace through crosshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{	
	//find crosshair position in pixel coodinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D(CrosshairXLocation * ViewportSizeX, CrosshairYLocation * ViewportSizeY);
	//deproject screen pos of crosshair to world direction.
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		//line-trace along this look direction. //see what we hit, up to a max range.
		GetLookVectorHitLocation(LookDirection, OUT HitLocation);
	}
	return true;
}



bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; //To be discarded.
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility))
	{
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0);
	return false;
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}
