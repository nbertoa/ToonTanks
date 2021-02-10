#include "TurretPawn.h"

#include "Kismet/GameplayStatics.h"
#include "TankPawn.h"

void
ATurretPawn::BeginPlay() {
	Super::BeginPlay();

	InitPlayerPawn();
	InitFireRateTimer();
}

void
ATurretPawn::HandleDestruction() {
	Super::HandleDestruction();

	Destroy();
}

void
ATurretPawn::Tick(float aDeltaTime) {
	Super::Tick(aDeltaTime);
	if (IsPlayerInRange() == false) {
		return;
	}
	const FVector playerLocation = mPlayerPawn->GetActorLocation();
	RotateTurret(playerLocation);
}

void 
ATurretPawn::InitPlayerPawn() {
	check(mPlayerPawn == nullptr);
	APawn* pawn = UGameplayStatics::GetPlayerPawn(this,
												  0); // Player index
	mPlayerPawn = Cast<ATankPawn>(pawn);
}

void 
ATurretPawn::InitFireRateTimer() {
	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.SetTimer(mFireRateTimer,
						  this,
						  &ATurretPawn::CheckFireCondition,
						  mFireRate,
						  true); // Loop
}

void 
ATurretPawn::CheckFireCondition() {
	if (mPlayerPawn == nullptr ||
		mPlayerPawn->IsPlayerAlive() == false) {
		return;
	}

	if (IsPlayerInRange()) {
		Fire();
	}
}

float 
ATurretPawn::GetDistanceToPlayer() const {
	check(mPlayerPawn != nullptr);

	const FVector playerPosition = mPlayerPawn->GetActorLocation();
	const FVector ourPosition = GetActorLocation();

	return FVector::Dist(playerPosition,
						 ourPosition);
}

bool 
ATurretPawn::IsPlayerInRange() const {
	return mPlayerPawn != nullptr && GetDistanceToPlayer() <= mFireRange;
}