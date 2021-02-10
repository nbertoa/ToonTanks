#include "TankGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Pawns/TankPawn.h"
#include "ToonTanks/Pawns/TurretPawn.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"

void 
ATankGameModeBase::ActorDied(AActor* aActor) {
    check(aActor != nullptr);

    if (aActor == mPlayerPawn) {
        mPlayerPawn->HandleDestruction();
        HandleGameOver(false); // Player lost
    } else {
        ATurretPawn* turret = Cast<ATurretPawn>(aActor);
        if (turret != nullptr) {
            turret->HandleDestruction();

            if (--mTurretCount == 0) {
                HandleGameOver(true); // Player won
            }
        }        
    }    
}

void 
ATankGameModeBase::BeginPlay() {
    Super::BeginPlay();

    HandleGameStart();
}

void 
ATankGameModeBase::HandleGameStart() {
    InitTurretCount();
    InitPlayerPawn();
    InitPlayerController();
    GameStart();
}

void 
ATankGameModeBase::HandleGameOver(const bool aDidPlayerWin) {
    if (mPlayerController != nullptr) {
        mPlayerController->SetPlayerEnabledState(aDidPlayerWin);
    }

    GameOver(aDidPlayerWin);
}

void 
ATankGameModeBase::InitTurretCount() {
    TArray<AActor*> turrets;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(),
                                          ATurretPawn::StaticClass(),
                                          turrets);
    mTurretCount = turrets.Num();
}

void 
ATankGameModeBase::InitPlayerPawn() {
    check(mPlayerPawn == nullptr);
    APawn* playerPawn = UGameplayStatics::GetPlayerPawn(this,
                                                        0);
    mPlayerPawn = Cast<ATankPawn>(playerPawn);
}

void 
ATankGameModeBase::InitPlayerController() {
    check(mPlayerController == nullptr);
    APlayerController* playerController = UGameplayStatics::GetPlayerController(this,
                                                                                0);
    mPlayerController = Cast<APlayerControllerBase>(playerController);

    if (mPlayerController != nullptr) {
        mPlayerController->SetPlayerEnabledState(false);
        SetTimerToEnablePlayer();
    }
}

void 
ATankGameModeBase::SetTimerToEnablePlayer() {
    check(mPlayerController != nullptr);

    FTimerHandle enablePlayerTimer;
    FTimerDelegate delegate = FTimerDelegate::CreateUObject(mPlayerController,
                                                            &APlayerControllerBase::SetPlayerEnabledState,
                                                            true); // Enable player 
    FTimerManager& timerManager = GetWorld()->GetTimerManager();
    timerManager.SetTimer(enablePlayerTimer,
                          delegate,
                          mPlayerInputStartDelay,
                          false);
}