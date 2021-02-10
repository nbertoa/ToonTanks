#include "ProjectileBase.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AProjectileBase::AProjectileBase() {
 	PrimaryActorTick.bCanEverTick = false;    
    InitComponents();
}

void AProjectileBase::BeginPlay() {
	Super::BeginPlay();

    mProjectileMesh->OnComponentHit.AddDynamic(this,
                                               &AProjectileBase::OnHit);

    PlaySound(mLaunchSound);
}

void 
AProjectileBase::InitComponents() {
    check(mProjectileMesh == nullptr);
    mProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
    RootComponent = mProjectileMesh;

    check(mProjectileMovement == nullptr);    
    mProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projective Movement"));
    mProjectileMovement->InitialSpeed = mMovementSpeed;
    mProjectileMovement->MaxSpeed = mMovementSpeed;
    InitialLifeSpan = mInitialLifeSpan;

    check(mTrailingParticleSystem == nullptr);
    mTrailingParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("Trailing Particle System");
    mTrailingParticleSystem->SetupAttachment(RootComponent);
}

void 
AProjectileBase::PlaySound(USoundBase* aSound) {
    if (aSound == nullptr) {
        return;
    }

    UGameplayStatics::PlaySoundAtLocation(this,
                                          aSound,
                                          GetActorLocation());
}

void 
AProjectileBase::OnHit(UPrimitiveComponent* aHitComponent,
                       AActor* aOtherActor,
                       UPrimitiveComponent* aOtherComp,
                       FVector aNormalImpulse,
                       const FHitResult& aHit) {
    AActor* owner = GetOwner();
    if (owner == nullptr) {
        return;
    }

    check(aOtherActor != nullptr);
    if (aOtherActor != this && aOtherActor != owner) {
        UGameplayStatics::ApplyDamage(aOtherActor,
                                      mDamage,
                                      owner->GetInstigatorController(),
                                      this,
                                      mDamageType);

        SpawnHitParticleSystem();

        PlaySound(mHitSound);

        GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(mHitShake);

        Destroy();
    }    
}

void 
AProjectileBase::SpawnHitParticleSystem() {
    if (mHitParticleSystem == nullptr) {
        return;
    }

    UGameplayStatics::SpawnEmitterAtLocation(this,
                                             mHitParticleSystem,
                                             GetActorLocation());
}
