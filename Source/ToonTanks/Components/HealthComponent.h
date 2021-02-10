#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class ATankGameModeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

protected:
	void BeginPlay() override;
	void InitGameMode();

	UFUNCTION()
	void TakeDamage(AActor* aDamagedActor, 
					float aDamage, 
					const UDamageType* aDamageType, 
					class AController* aInstigatedBy, 
					AActor* aDamageCauser);

private:
	UPROPERTY(EditAnywhere)
	float mDefaultHealth = 100.0f;
	float mCurrentHealth = 0.0f;

	ATankGameModeBase* mGameMode = nullptr;
};
