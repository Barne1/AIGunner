// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIGunner/OlofsCode/AI/StateMachine/FGAIStateBase.h"
#include "HAI_IdlePatrol.generated.h"

class AFGEnemy;

UCLASS()
class AIGUNNER_API UHAI_IdlePatrol : public UFGAIStateBase
{
	GENERATED_BODY()
public:
	UHAI_IdlePatrol();

protected:
	AFGEnemy* OwnerEnemy;

public:
	virtual void Setup() override;
	virtual void OnActivated() override;
	virtual void OnDeactivated() override;
	virtual void OnNoiseHeard(const FNoiseInfo& NoiseInfo) override;
};
