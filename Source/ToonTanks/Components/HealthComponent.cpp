#include "HealthComponent.h"

#include "ToonTanks/GameModes/TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"

UHealthComponent::UHealthComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void 
UHealthComponent::BeginPlay() {
	Super::BeginPlay();

	mCurrentHealth = mDefaultHealth;
	InitGameMode();
}

void 
UHealthComponent::InitGameMode() {
	check(mGameMode == nullptr);

	AGameModeBase* gameMode = UGameplayStatics::GetGameMode(GetWorld());
	mGameMode = Cast<ATankGameModeBase>(gameMode);
	GetOwner()->OnTakeAnyDamage.AddDynamic(this,
										   &UHealthComponent::TakeDamage);
}

void 
UHealthComponent::TakeDamage(AActor* aDamagedActor,
							 float aDamage,
							 const UDamageType* aDamageType,
							 class AController* aInstigatedBy,
							 AActor* aDamageCauser) {
	check(aDamagedActor != nullptr);
	check(aDamage >= 0.0f);
	check(aDamageType != nullptr);
	check(aInstigatedBy != nullptr);
	check(aDamageCauser != nullptr);

	if (mGameMode == nullptr) {
		UE_LOG(LogTemp,
			   Warning,
			   TEXT("Health component does not have a GameModeBase"));
	}

	if (mCurrentHealth <= 0.0f) {
		return;
	}

	mCurrentHealth = FMath::Clamp(mCurrentHealth - aDamage,
								  0.0f,
								  mDefaultHealth);

	if (mCurrentHealth <= 0.0f && mGameMode != nullptr) {
		mGameMode->ActorDied(GetOwner());
	}
}

