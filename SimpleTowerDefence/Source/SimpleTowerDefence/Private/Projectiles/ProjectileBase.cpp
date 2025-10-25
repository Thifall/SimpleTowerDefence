// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectileBase::AProjectileBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create and setup the collision sphere
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionComponent->InitSphereRadius(15.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
    RootComponent = CollisionComponent;

    // Create and setup the projectile mesh
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(CollisionComponent);
    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Set defaults
    bHasImpacted = false;
}

void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();
    
    // Bind hit event
    CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnProjectileHit);
    
    // Set lifetime
    SetLifeSpan(MaxLifeTime);
}

void AProjectileBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bHasImpacted && Target)
    {
        // Move towards target
        FVector Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        FVector NewLocation = GetActorLocation() + Direction * Speed * DeltaTime;
        SetActorLocation(NewLocation);

        // Update rotation to face movement direction
        SetActorRotation(Direction.Rotation());
    }
}

void AProjectileBase::SetTarget(AEnemyBase* InTarget)
{
    Target = InTarget;
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!bHasImpacted && OtherActor != this)
    {
        bHasImpacted = true;

        // Apply damage if we hit our target
        if (AEnemyBase* HitEnemy = Cast<AEnemyBase>(OtherActor))
        {
            UGameplayStatics::ApplyDamage(HitEnemy, Damage, GetInstigatorController(), this, nullptr);
        }

        // Destroy the projectile
        Destroy();
    }
}