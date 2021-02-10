#include "PawnBase.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Actors/ProjectileBase.h"
#include "ToonTanks/Components/HealthComponent.h"

APawnBase::APawnBase() {
	PrimaryActorTick.bCanEverTick = true;

	InitComponents();
}

void 
APawnBase::BeginPlay() {
	Super::BeginPlay();
}

void 
APawnBase::RotateTurret(const FVector& aLookAtTargetLocation) {

	const FVector startLocation = mTurretMesh->GetComponentLocation();
	
	// We "clean" the aLookAtTargetLocation by using the same
	// Z coordinate of the turret mesh, so it is going to rotate
	// without changing the height.
	const FVector endLocation = FVector(aLookAtTargetLocation.X,
										aLookAtTargetLocation.Y,
										startLocation.Z);

	const FVector fromStartToEndVector = endLocation - startLocation;
	const FRotator rotation = fromStartToEndVector.Rotation();
	mTurretMesh->SetWorldRotation(rotation);
}

void 
APawnBase::Fire() {
	SpawnProjectile();
}

void 
APawnBase::HandleDestruction() {
	EmitParticles(mDeathParticleSystem);

	PlaySound(mDeathSound);

	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(mDeathShake);
}

void 
APawnBase::Tick(float aDeltaTime) {
	Super::Tick(aDeltaTime);
}

void 
APawnBase::SetupPlayerInputComponent(UInputComponent* aPlayerInputComponent) {
	Super::SetupPlayerInputComponent(aPlayerInputComponent);
}

void 
APawnBase::InitComponents() {
	check(mCapsuleComponent == nullptr);
	mCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = mCapsuleComponent;

	check(mBaseMesh == nullptr);
	mBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	mBaseMesh->SetupAttachment(RootComponent);

	check(mTurretMesh == nullptr);
	mTurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	mTurretMesh->SetupAttachment(mBaseMesh);

	check(mProjectileSpawnPoint == nullptr);
	mProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	mProjectileSpawnPoint->SetupAttachment(mTurretMesh);

	check(mHealth == nullptr);
	mHealth = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

void 
APawnBase::SpawnProjectile() {
	if (mProjectileClass.Get() == nullptr) {
		return;
	}

	check(mProjectileSpawnPoint != nullptr);

	const FVector spawnLocation = mProjectileSpawnPoint->GetComponentLocation();
	const FRotator spawnRotation = mProjectileSpawnPoint->GetComponentRotation();

	AProjectileBase* projectile = GetWorld()->SpawnActor<AProjectileBase>(mProjectileClass,
																		  spawnLocation,
																		  spawnRotation);
	projectile->SetOwner(this);
}

void 
APawnBase::EmitParticles(UParticleSystem* aParticleSystem) {
	if (aParticleSystem == nullptr) {
		return;
	}

	UGameplayStatics::SpawnEmitterAtLocation(this,
											 aParticleSystem,
											 GetActorLocation());
}

void 
APawnBase::PlaySound(USoundBase* aSound) {
	if (aSound == nullptr) {
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(this,
										  aSound,
										  GetActorLocation());
}

