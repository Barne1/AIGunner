// Fill out your copyright notice in the Description page of Project Settings.

#include "AIGunner/HugoStates/HAI_IdlePatrol.h"
#include "AIGunner/OlofsCode/FGEnemy.h"
#include "AIGunner/FGPatrolComponent.h"

UHAI_IdlePatrol::UHAI_IdlePatrol()
{
	StateName = "IdlePatrol";
	PendingStateName = NAME_None; //Not sure if this is needed
}

void UHAI_IdlePatrol::Setup()
{
	OwnerEnemy = Cast<AFGEnemy>(Owner);

	Super::Setup();
}

void UHAI_IdlePatrol::OnActivated()
{
	if(OwnerEnemy->PatrolComp)
	{
		OwnerEnemy->PatrolComp->StartPatrol();
	}
	Super::OnActivated();
}

void UHAI_IdlePatrol::OnDeactivated()
{
	if(OwnerEnemy->PatrolComp)
	{
		OwnerEnemy->PatrolComp->StopPatrol();
	}
	Super::OnDeactivated();
}

void UHAI_IdlePatrol::OnNoiseHeard(const FNoiseInfo& NoiseInfo)
{
	ChangeState("Alert");
}
