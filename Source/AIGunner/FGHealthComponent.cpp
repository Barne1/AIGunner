// Fill out your copyright notice in the Description page of Project Settings.


#include "FGHealthComponent.h"

// Sets default values for this component's properties
UFGHealthComponent::UFGHealthComponent()
{
}


// Called when the game starts
void UFGHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

float UFGHealthComponent::GetHealthPercentage()
{
	return CurrentHealth/MaxHealth;
}

void UFGHealthComponent::TakeDamage(int Damage)
{
	if(!bAlive)
		return;

	OnTakeDamage.Broadcast(Damage);
	
	CurrentHealth -= Damage;
	CurrentHealth = FMath::Max(0, CurrentHealth);
	if(CurrentHealth <= 0)
	{
		bAlive = false;
		OnDeath.Broadcast();
	}
}

