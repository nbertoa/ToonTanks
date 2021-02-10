#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "TurretPawn.generated.h"

class ATankPawn;

UCLASS()
class TOONTANKS_API ATurretPawn : public APawnBase {
	GENERATED_BODY()

public:
	void Tick(float aDeltaTime) override;

	void HandleDestruction() override;

protected:
	void BeginPlay() override;	

private:
	void InitPlayerPawn();
	void InitFireRateTimer();
	void CheckFireCondition();
	float GetDistanceToPlayer() const;
	bool IsPlayerInRange() const;

	ATankPawn* mPlayerPawn = nullptr;

	FTimerHandle mFireRateTimer;

	UPROPERTY(VisibleAnywhere,
			  BlueprintReadOnly,
			  Category = "Combat",
			  meta = (AllowPrivateAccess = "true"))
	float mFireRate = 2.0f;

	UPROPERTY(VisibleAnywhere,
			  BlueprintReadOnly,
			  Category = "Combat",
			  meta = (AllowPrivateAccess = "true"))
	float mFireRange = 500.0f;
};
