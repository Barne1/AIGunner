// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FGHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTakeDamageDelegate, int, DamageTaken);

//This class is intended to be hooked into with Blueprints
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIGUNNER_API UFGHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFGHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaxHealth = 100;
	UPROPERTY(BlueprintReadOnly)
	int CurrentHealth;

public:
	UFUNCTION(BlueprintCallable)
	float GetHealthPercentage();

	UFUNCTION(BlueprintCallable)
	void TakeDamage(int Damage);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bAlive = true;
	
	UPROPERTY(BlueprintAssignable)
	FEnemyDeathDelegate OnDeath;

	UPROPERTY(BlueprintAssignable)
	FTakeDamageDelegate OnTakeDamage;
	
};
