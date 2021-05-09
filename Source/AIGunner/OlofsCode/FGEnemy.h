#pragma once

#include "GameFramework/Pawn.h"
#include "FGEnemy.generated.h"

class UFGNoiseMakerComponent;
class UFGHearingComponent;
class UEnemyGunComponent;
class AAIController;
class UFGStateMachineComponent;
class UFGPatrolComponent;
class USkeletalMeshComponent;
class UCapsuleComponent;
class UCameraComponent;
class UFGVisionSensingComponent;
class UFGNavMovementComponent;

UCLASS()
class AFGEnemy : public APawn
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Collision)
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Vision)
	UFGVisionSensingComponent* VisionSensingComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Movement)
	UFGNavMovementComponent* NavMovementComponent;
	
	//Addition
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFGPatrolComponent* PatrolComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFGStateMachineComponent* StateMachineComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = AI)
	AAIController* AiController;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Gun)
	UEnemyGunComponent* GunComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hearing)
	UFGHearingComponent* HearingComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Noise)
	UFGNoiseMakerComponent* NoiseMakerComponent;

	AFGEnemy();

	virtual float GetDefaultHalfHeight() const override;

protected:
	virtual void BeginPlay();

	UCapsuleComponent* GetCapsule() const { return Capsule; }
};

