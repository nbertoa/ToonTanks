#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnBase.generated.h"

class AProjectileBase;
class UCapsuleComponent;
class UHealthComponent;
class UParticleSystem;
class USceneComponent;
class USoundBase;
class UStaticMeshComponent;

UCLASS()
class TOONTANKS_API APawnBase : public APawn {
	GENERATED_BODY()

public:
	APawnBase();

	void Tick(float aDeltaTime) override;

	void SetupPlayerInputComponent(class UInputComponent* aPlayerInputComponent) override;

	virtual void HandleDestruction();

protected:
	void BeginPlay() override;

	virtual void RotateTurret(const FVector& aLookAtTargetLocation);
	virtual void Fire();

private:
	void InitComponents();
	void SpawnProjectile();
	void EmitParticles(UParticleSystem* aParticleSystem);
	void PlaySound(USoundBase* aSound);

	// General collision for the pawn
	UPROPERTY(VisibleAnywhere,
			  BlueprintReadOnly,
			  Category = "Components",
			  meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* mCapsuleComponent = nullptr;

	UPROPERTY(VisibleAnywhere,
			  BlueprintReadOnly,
			  Category = "Components",
			  meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mBaseMesh = nullptr;
	
	UPROPERTY(VisibleAnywhere,
			  BlueprintReadOnly,
			  Category = "Components",
			  meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mTurretMesh = nullptr;

	UPROPERTY(VisibleAnywhere,
			  BlueprintReadOnly,
			  Category = "Components",
			  meta = (AllowPrivateAccess = "true"))
	USceneComponent* mProjectileSpawnPoint = nullptr;

	UPROPERTY(EditAnywhere,
			  BlueprintReadOnly,
			  Category = "Projectile Type",
			  meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> mProjectileClass;

	UPROPERTY(EditAnywhere,
			  Category = "Effects")
	UParticleSystem* mDeathParticleSystem = nullptr;

	UPROPERTY(VisibleAnywhere,
			  BlueprintReadOnly,
			  Category = "Components",
			  meta = (AllowPrivateAccess = "true"))
	UHealthComponent* mHealth = nullptr;

	UPROPERTY(EditAnywhere,
			  Category = "Effects")
	USoundBase* mDeathSound = nullptr;

	UPROPERTY(EditAnywhere,
			  Category = "Effects")
	TSubclassOf<UCameraShake> mDeathShake;
};
