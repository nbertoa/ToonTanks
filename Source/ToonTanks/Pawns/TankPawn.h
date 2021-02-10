#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "TankPawn.generated.h"

class APlayerController;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TOONTANKS_API ATankPawn : public APawnBase
{
	GENERATED_BODY()

public:
	ATankPawn();

	void Tick(float aDeltaTime) override;

	void SetupPlayerInputComponent(class UInputComponent* aPlayerInputComponent) override;

	void HandleDestruction() override;

	bool IsPlayerAlive() const { return mIsPlayerAlive; }

protected:
	void BeginPlay() override;	

private:
	void InitComponents();
	void InitPlayerController();

	void CalculateMovementInput(const float aValue);
	void CalculateRotationInput(const float aValue);

	void Move();
	void RotateBody();

	void RotateTurretToMouseCursorLocation();

	APlayerController* mPlayerController = nullptr;

	UPROPERTY(VisibleAnywhere,
			  BlueprintReadOnly,
			  Category = "Components",
			  meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* mSpringArm = nullptr;

	UPROPERTY(VisibleAnywhere,
			  BlueprintReadOnly,
			  Category = "Components",
			  meta = (AllowPrivateAccess = "true"))
	UCameraComponent* mCamera = nullptr;

	FVector mMovementDirection = FVector(0.0f, 
										 0.0f,
										 0.0f);

	FQuat mRotationDirection = FQuat(0.0f,
									 0.0f,
									 0.0f,
									 1.0f);

	UPROPERTY(EditAnywhere,
			  BlueprintReadWrite,
			  Category = "Movement",
			  meta = (AllowPrivateAccess = "true"))
	float mMovementSpeed = 100.0f;

	UPROPERTY(EditAnywhere, 
			  BlueprintReadWrite,
			  Category = "Movement",
			  meta = (AllowPrivateAccess = "true"))
	float mRotationSpeed = 100.0f;

	bool mIsPlayerAlive = true;
};
