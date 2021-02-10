#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGameModeBase.generated.h"

class APlayerControllerBase;
class ATankPawn;
class ATurretPawn;

UCLASS()
class TOONTANKS_API ATankGameModeBase : public AGameModeBase {
	GENERATED_BODY()

public:
	void ActorDied(AActor* aActor);

protected:
	void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(const bool aDidPlayerWin);

private:
	void HandleGameStart();
	void HandleGameOver(const bool aDidPlayerWin);

	void InitTurretCount();
	void InitPlayerPawn();
	void InitPlayerController();

	void SetTimerToEnablePlayer();

	ATankPawn* mPlayerPawn = nullptr;
	uint32 mTurretCount = 0;

	APlayerControllerBase* mPlayerController = nullptr;

	UPROPERTY(EditAnywhere)
	float mPlayerInputStartDelay = 3.0f;
};
