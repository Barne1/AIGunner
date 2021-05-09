#pragma once

#include "Components/ActorComponent.h"
#include "FGVisionSensingComponent.generated.h"

class AActor;
class UFGVisionSensingTargetComponent;
class UFGVisionSensingSettings;

USTRUCT(BlueprintType)
struct FFGVisionSensingResults
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	AActor* SensedActor = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFGVisionSensingDelegate, const FFGVisionSensingResults&, Results);

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class UFGVisionSensingComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UFGVisionSensingComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsPointVisible(const FVector& PointToTest, const FVector& Origin, const FVector& Direction, float DistanceMinimum, AActor* Senser, AActor* Sensed) const;

	UPROPERTY(Transient)
	TArray<UFGVisionSensingTargetComponent*> SensedTargets;

	float GetVisionInRadians() const;

	/*UPROPERTY(BlueprintAssignable)
	FFGVisionSensingDelegate OnTargetSensed;

	UPROPERTY(BlueprintAssignable)
	FFGVisionSensingDelegate OnTargetLost;*/

	UPROPERTY(EditAnywhere, Category = Sensing)
	UFGVisionSensingSettings* SensingSettings = nullptr;

	UPROPERTY(EditAnywhere, Category = Debug)
	bool bDebugDrawVision = false;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ETraceTypeQuery> VisionTraceChannel;

protected:
	bool BlockedByWall(AActor* Senser, AActor* Sensed) const;

	//Additions below
	UPROPERTY(EditAnywhere)
	float TimeUntilSuspicious = 0.5f;
	UPROPERTY(EditAnywhere)
	float TimeUntilAlert = 1.5f;

	float TimeVisible = 0; //The time the player has been in the enemys sight

public:
	UFUNCTION(BlueprintCallable)
	float GetPercentAlerted();

	UFUNCTION(BlueprintCallable)
    float GetPercentSuspicious();

	UPROPERTY(BlueprintAssignable)
	FFGVisionSensingDelegate OnAlert;
	UPROPERTY(BlueprintAssignable)
	FFGVisionSensingDelegate OnSuspicious;
	UPROPERTY(BlueprintAssignable)
	FFGVisionSensingDelegate OnPlayerSeen;
	UPROPERTY(BlueprintAssignable)
	FFGVisionSensingDelegate OnPlayerLost;

	UPROPERTY(BlueprintReadOnly)
	FVector LastSeenPosition;

	UPROPERTY(BlueprintReadOnly)
	bool bAlerted = false;

	UPROPERTY(BlueprintReadOnly)
	bool bCanSeePlayer;

	bool bActivated = true;
	void DisableVision();
	void EnableVision();
};
