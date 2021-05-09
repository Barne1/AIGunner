// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FGNoiseMakerComponent.h"
#include "FGHearingComponent.generated.h"

struct FNoiseInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNoiseHeardDelegate, const FNoiseInfo&, NoiseInfo);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UFGHearingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFGHearingComponent();
protected:
	virtual void BeginPlay() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//TODO: Implement timer for how long to remember noise
	const FNoiseInfo* LastNoiseHeardInfo;

	//TODO: Implement reaction to noise
	UPROPERTY(BlueprintAssignable)
	FNoiseHeardDelegate OnNoiseHeard;
	
	void HearNoise(const FNoiseInfo& NoiseInfo);
};
