#include "Enemies/EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

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
}

void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize health
    CurrentHealth = MaxHealth;
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

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (!IsAlive() || DamageAmount <= 0.0f)
        return 0.0f;

    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    CurrentHealth = FMath::Max(0.0f, CurrentHealth - ActualDamage);

    // Handle damage events
    if (CurrentHealth > 0.0f)
    {
        OnEnemyDamaged.Broadcast(ActualDamage, CurrentHealth);
        OnDamaged(ActualDamage, CurrentHealth);
    }
    else
    {
        OnEnemyDied.Broadcast(this);
        OnDeath();
    }

    return ActualDamage;
}

void AEnemyBase::OnDeath_Implementation()
{
    // Base implementation simply destroys the actor
    // Override in BP to add effects/rewards before destroying
    Destroy();
}

void AEnemyBase::OnDamaged_Implementation(float DamageAmount, float HealthRemaining)
{
    // Base implementation does nothing
    // Override in BP to add damage effects/feedback
}

void AEnemyBase::OnReachedEnd_Implementation()
{
    // Base implementation simply destroys the actor
    // Override in BP to handle reaching the end (e.g. damage player base)
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