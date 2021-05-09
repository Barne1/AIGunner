#include "FGEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AI/Sensing/FGVisionSensingComponent.h"
#include "AI/Movement/FGNavMovementComponent.h"
#include "AI/StateMachine/FGStateMachineComponent.h"
#include "AIGunner/FGPatrolComponent.h"
#include "AIController.h"
#include "AIGunner/EnemyGunComponent.h"
#include "AIGunner/OlofsCode/AI/Sensing/FGHearingComponent.h"
#include "AI/Sensing/FGNoiseMakerComponent.h"

AFGEnemy::AFGEnemy()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;
	// Set size for collision capsule
	Capsule->InitCapsuleSize(55.f, 96.0f);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh->SetupAttachment(Capsule);
	Mesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));

	VisionSensingComponent = CreateDefaultSubobject<UFGVisionSensingComponent>(TEXT("VisionSensing"));
	NavMovementComponent = CreateDefaultSubobject<UFGNavMovementComponent>(TEXT("NavMovementComponent"));

	PatrolComp = CreateDefaultSubobject<UFGPatrolComponent>(TEXT("PatrolComponent"));
	StateMachineComp = CreateDefaultSubobject<UFGStateMachineComponent>(TEXT("StateMachineComponent"));

	GunComponent = CreateDefaultSubobject<UEnemyGunComponent>(TEXT("GunComponent"));
	HearingComponent = CreateDefaultSubobject<UFGHearingComponent>(TEXT("HearingComponent"));
	NoiseMakerComponent = CreateDefaultSubobject<UFGNoiseMakerComponent>(TEXT("NoiseMakerComponent"));
}

float AFGEnemy::GetDefaultHalfHeight() const
{
	// Don't scale...
	return Capsule->GetScaledCapsuleHalfHeight();
}

void AFGEnemy::BeginPlay()
{
	Super::BeginPlay();

	AiController = Cast<AAIController>(GetController());

}
