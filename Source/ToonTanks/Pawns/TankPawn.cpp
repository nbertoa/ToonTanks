#include "TankPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ATankPawn::ATankPawn() {
	InitComponents();
}

void
ATankPawn::BeginPlay() {
	Super::BeginPlay();

	InitPlayerController();
}

void 
ATankPawn::HandleDestruction() {
	Super::HandleDestruction();

	mIsPlayerAlive = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void
ATankPawn::Tick(float aDeltaTime) {
	Super::Tick(aDeltaTime);

	Move();
	RotateBody();
	RotateTurretToMouseCursorLocation();
}

void
ATankPawn::SetupPlayerInputComponent(UInputComponent* aPlayerInputComponent) {
	Super::SetupPlayerInputComponent(aPlayerInputComponent);

	aPlayerInputComponent->BindAxis("MoveForward",
									this,
									&ATankPawn::CalculateMovementInput);

	aPlayerInputComponent->BindAxis("Turn",
									this,
									&ATankPawn::CalculateRotationInput);

	aPlayerInputComponent->BindAction("Fire",
									  IE_Pressed,
									  this,
									  &ATankPawn::Fire);
}

void
ATankPawn::InitComponents() {
	check(mSpringArm == nullptr);
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	mSpringArm->SetupAttachment(RootComponent);

	check(mCamera == nullptr);
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(mSpringArm);
}

void 
ATankPawn::InitPlayerController() {
	check(mPlayerController == nullptr);

	mPlayerController = Cast<APlayerController>(GetController());
}

void 
ATankPawn::CalculateMovementInput(const float aValue) {
	const float xMovement = aValue * mMovementSpeed * GetWorld()->GetDeltaSeconds();
	mMovementDirection = FVector(xMovement,
								 0.0f,
								 0.0f);
}

void 
ATankPawn::CalculateRotationInput(const float aValue) {
	const float yawRotation = aValue * mRotationSpeed * GetWorld()->GetDeltaSeconds();
	const FRotator rotation = FRotator(0.0,
									   yawRotation,
									   0.0f);
	mRotationDirection = FQuat(rotation);
}

void 
ATankPawn::Move() {
	AddActorLocalOffset(mMovementDirection,
						true); // Sweep
}

void 
ATankPawn::RotateBody() {
	AddActorLocalRotation(mRotationDirection,
						  true); // Sweep
}

void 
ATankPawn::RotateTurretToMouseCursorLocation() {
	if (mPlayerController == nullptr) {
		return;
	}

	FHitResult hitResult;
	mPlayerController->GetHitResultUnderCursor(ECC_Visibility,
											   false, // Trace complex
											   hitResult);

	const FVector& hitLocation = hitResult.ImpactPoint;
	RotateTurret(hitLocation);
}
