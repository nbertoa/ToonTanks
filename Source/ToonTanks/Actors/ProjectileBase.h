#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UParticleSystem;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class USoundBase;
class UStaticMeshComponent;

UCLASS()
class TOONTANKS_API AProjectileBase : public AActor {
	GENERATED_BODY()
	
public:	
	AProjectileBase();

protected:
	void BeginPlay() override;

private:
	void InitComponents();
	void PlaySound(USoundBase* aSound);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* aHitComponent,
			   AActor* aOtherActor, 
			   UPrimitiveComponent* aOtherComp, 
			   FVector aNormalImpulse, 
			   const FHitResult& aHit);

	void SpawnHitParticleSystem();

	UPROPERTY(VisibleAnywhere,
			  BlueprintReadOnly,
			  Category = "Components",
			  meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* mProjectileMovement = nullptr;

	UPROPERTY(VisibleAnywhere,
			  BlueprintReadOnly,
			  Category = "Components",
			  meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mProjectileMesh = nullptr;

	UPROPERTY(EditDefaultsOnly,
			  BlueprintReadOnly,
			  Category = "Damage",
			  meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> mDamageType;

	UPROPERTY(EditAnywhere,
			  BlueprintReadOnly,
			  Category = "Movement",
			  meta = (AllowPrivateAccess = "true"))
	float mMovementSpeed = 1300.0f;

	UPROPERTY(EditAnywhere,
			  BlueprintReadOnly,
			  Category = "Damage",
			  meta = (AllowPrivateAccess = "true"))
	float mDamage = 50.0f;

	UPROPERTY(EditAnywhere,
			  BlueprintReadOnly,
			  Category = "Life",
			  meta = (AllowPrivateAccess = "true"))
	float mInitialLifeSpan = 3.0f;

	UPROPERTY(EditAnywhere,
			  Category = "Effects")
	UParticleSystem* mHitParticleSystem = nullptr;

	UPROPERTY(VisibleAnywhere,
			  BlueprintReadOnly,
			  Category = "Components",
			  meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* mTrailingParticleSystem = nullptr;

	UPROPERTY(EditAnywhere,
			  Category = "Effects")
	USoundBase* mLaunchSound = nullptr;

	UPROPERTY(EditAnywhere,
			  Category = "Effects")
	USoundBase* mHitSound = nullptr;

	UPROPERTY(EditAnywhere,
			  Category = "Effects")
	TSubclassOf<UCameraShake> mHitShake;
	
};
