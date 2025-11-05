#include "Enemies/EnemyBase.h"
#include "UI/Enemies/EnemyHealthBarComponent.h"  
#include "Components/CapsuleComponent.h"
#include <Subsystems/ExperienceSubsystem.h>
#include <Subsystems/PlayerHPSubsystem.h>

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup collision
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = CapsuleComponent;

	// Setup mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EnemyUI = CreateDefaultSubobject<UEnemyHealthBarComponent>(TEXT("EnemyUI"));
	EnemyUI->SetupAttachment(RootComponent);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	// Initialize health
	CurrentHealth = MaxHealth;
	EnemyUI->SetWidgetClass(LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/Enemies/WBP_EnemyHealthBar.WBP_EnemyHealthBar_C")));
	EnemyUI->SetRelativeLocation(FVector(0.f, 0.f, 120.f)); // nad g³ow¹ przeciwnika
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlive())
	{
		UpdateMovement(DeltaTime);
	}
}

void AEnemyBase::SetPath(UPathComponent* path)
{
	PathComponent = path;
	CurrentWaypointIndex = 0;
}

float AEnemyBase::InflictDamage(float DamageAmount, AActor* DamageCauser)
{
	if (!IsAlive() || DamageAmount <= 0.0f)
	{
		return 0.0f;
	}

	CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);
	if (EnemyUI)
	{
		EnemyUI->UpdateHealthBar(CurrentHealth, MaxHealth);
	}
	// Handle damage events
	if (CurrentHealth > 0.0f)
	{
		OnEnemyDamaged.Broadcast(DamageAmount, CurrentHealth);
	}
	else
	{
		OnEnemyDied.Broadcast(this);
		OnDeath();
	}

	return DamageAmount;
}

void AEnemyBase::OnDeath_Implementation()
{
	UExperienceSubsystem* expSubsystem = GetWorld()->GetSubsystem<UExperienceSubsystem>();
	if (expSubsystem)
	{
		expSubsystem->AddExperiencePoints(ExperienceReward);
	}
	Destroy();
}

void AEnemyBase::OnReachedEnd_Implementation()
{
	UPlayerHPSubsystem* playerHPSubsystem = GetWorld()->GetSubsystem<UPlayerHPSubsystem>();
	playerHPSubsystem->DecreaseHealth(1);
	Destroy();
}

void AEnemyBase::UpdateMovement(float DeltaTime)
{
	if (!PathComponent || PathComponent->PathNodes.Num() == 0)
	{
		return;
	}

	if (!PathComponent->PathNodes.IsValidIndex(CurrentWaypointIndex))
	{
		return;
	}

	// Get current waypoint
	AActor* CurrentWaypoint = PathComponent->PathNodes[CurrentWaypointIndex];
	if (!CurrentWaypoint)
	{
		return;
	}

	// Calculate movement
	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = CurrentWaypoint->GetActorLocation();
	FVector DirectionToTarget = (TargetLocation - CurrentLocation).GetSafeNormal();

	// Rotate to face movement direction
	FRotator NewRotation = DirectionToTarget.Rotation();
	SetActorRotation(NewRotation);

	// Move towards waypoint
	float DistanceToTarget = FVector::Distance(CurrentLocation, TargetLocation);
	if (DistanceToTarget <= ReachThreshold)
	{
		// Reached waypoint - move to next one
		CurrentWaypointIndex++;

		if (IsAtFinalWaypoint())
		{
			OnReachedEnd();
			return;
		}
	}
	else
	{
		// Continue moving towards current waypoint
		float MovementStep = MovementSpeed * DeltaTime;
		FVector NewLocation = CurrentLocation + DirectionToTarget * FMath::Min(MovementStep, DistanceToTarget);
		SetActorLocation(NewLocation);
	}
}

bool AEnemyBase::IsAtFinalWaypoint() const
{
	return PathComponent && !PathComponent->PathNodes.IsValidIndex(CurrentWaypointIndex);
}