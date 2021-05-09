// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGunner/HugoStates/HAI_Dead.h"
#include "AIController.h"
#include "AIGunner/EnemyGunComponent.h"
#include "AIGunner/FGPatrolComponent.h"
#include "AIGunner/OlofsCode/FGEnemy.h"
#include "AIGunner/OlofsCode/AI/Sensing/FGVisionSensingComponent.h"

UHAI_Dead::UHAI_Dead()
{
	StateName = "Dead";
	PendingStateName = NAME_None; //Not sure if this is needed
}

void UHAI_Dead::Setup()
{
	OwnerEnemy = Cast<AFGEnemy>(Owner);
	ensure(OwnerEnemy != nullptr);

	Super::Setup();
}

void UHAI_Dead::OnActivated()
{
	OwnerEnemy->Controller->StopMovement();
	OwnerEnemy->PatrolComp->StopPatrol();
	OwnerEnemy->VisionSensingComponent->DisableVision();
	OwnerEnemy->GunComponent->StopFire();
}
