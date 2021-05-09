// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIGunner/OlofsCode/AI/StateMachine/FGAIStateBase.h"
#include "HAI_Dead.generated.h"

class AAIController;
class AFGEnemy;

UCLASS()
class AIGUNNER_API UHAI_Dead : public UFGAIStateBase
{
	GENERATED_BODY()
public:
	UHAI_Dead();
	virtual void Setup() override;
	virtual void OnActivated() override;

protected:
	AFGEnemy* OwnerEnemy;
};
