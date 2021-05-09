// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIGunner/OlofsCode/AI/Sensing/FGNoiseMakerComponent.h"
#include "GameFramework/GameStateBase.h"
#include "FGGameState.generated.h"

class UFGHearingComponent;

UCLASS()
class AFGGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	TArray<UFGHearingComponent*> HearingComponents;
	void AlertListeners(FNoiseInfo NoiseInfo);
};
