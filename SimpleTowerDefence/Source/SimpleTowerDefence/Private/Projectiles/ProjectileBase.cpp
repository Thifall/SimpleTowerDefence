// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create and setup the collision sphere
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnProjectileBeginOverlap);

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
	Target->OnDestroyed.AddDynamic(this, &AProjectileBase::OnTargetDestroyed);
}

void AProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//tu mi agent podpowiedzia³ jakiœ miks walidacji, sprawdzamy czy nasze wskaŸniki s¹ OK i czy przypadkiem ju¿ siê nie "zderzyliœmy"
	if (!OtherActor || !OtherComp || bHasImpacted)
	{
		return;
	}

	//czy trafiliœmy w przeciwnika
	AEnemyBase* HitEnemy = Cast<AEnemyBase>(OtherActor);
	if (!HitEnemy)
	{
		return;
	}
	//czy ten przeciwnik, to rzeczywiœcie nasz cel
	if (Target)
	{
		if (HitEnemy != Target)
		{
			return;
		}
	}
	bHasImpacted = true;

	// Apply damage to the enemy
	HitEnemy->InflictDamage(Damage, this);

	// Destroy the projectile
	CollisionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AProjectileBase::OnProjectileBeginOverlap);
	Destroy();
}

void AProjectileBase::OnTargetDestroyed(AActor* DestroyedActor)
{
	// defensive check
	if (DestroyedActor == Target)
	{
		Target = nullptr;
		//Na ten moment mo¿na po prostu rozwaliæ, 
		//w przysz³oœci mo¿na zapamiêtaæ ostatni¹ lokacjê celu i dolecieæ "do ziemi" tak gdzie cel sta³ jak zosta³ zniszczony
		Destroy();
	}
}