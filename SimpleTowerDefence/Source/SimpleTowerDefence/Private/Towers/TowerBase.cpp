#include "Towers/TowerBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATowerBase::ATowerBase()
{
	RootMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	SetRootComponent(RootMeshComponent);

	EnemyDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemyDetection"));
	EnemyDetection->SetupAttachment(RootComponent);
	EnemyDetection->SetCollisionProfileName(TEXT("OverlapAll"));
	EnemyDetection->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EnemyDetection->SetGenerateOverlapEvents(true);

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_timeSinceLastAttack = AttackSpeed;
}

// Called when the game starts or when spawned
void ATowerBase::BeginPlay()
{
	Super::BeginPlay();

	EnemyDetection->OnComponentBeginOverlap.AddDynamic(this, &ATowerBase::OnEnemyBeginOverlap);
	EnemyDetection->OnComponentEndOverlap.AddDynamic(this, &ATowerBase::OnEnemyEndOverlap);
	return;
}

// Called every frame
void ATowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AttackTarget(DeltaTime);
}

void ATowerBase::OnEnemyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyBase* enemy = Cast<AEnemyBase>(OtherActor);
	if (!enemy)
	{
		return;
	}
	// jak przeciwnik padnie, to musimy o tym wiedzieæ, ¿eby usun¹æ go z listy targetów
	enemy->OnDestroyed.AddDynamic(this, &ATowerBase::OnTargetDestroyed);
	_enemiesInRange.AddUnique(enemy);

	//jak nie mamy innego celu, to ogieñ
	if (!_currentTarget)
	{
		//na ten moment robimy to po prostu tak, ale mo¿e lepiej mimo wszystko braæ przeciwnika z indeksu 0? 
		//gdyby overlap zbieg³ siê w czasie to mo¿emy ztargetowaæ œwie¿o "namierzonego" przeciwnika zamiast tego dalej posuniêtego na drodze
		_currentTarget = enemy;
	}
}

void ATowerBase::OnEnemyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AEnemyBase* enemy = Cast<AEnemyBase>(OtherActor);
	if (!enemy)
	{
		return;
	}
	//jak przeciwnik poza zasiêgiem, to nie potrzebujemy o nim dalszych informacji
	enemy->OnDestroyed.RemoveDynamic(this, &ATowerBase::OnTargetDestroyed);

	//jeœli to by³ nasz cel, to pora zmieniæ cel
	if (_currentTarget && OtherActor == _currentTarget)
	{
		_currentTarget = nullptr;
	}
	//wywalamy z listy targetów
	_enemiesInRange.Remove(enemy);
}

void ATowerBase::OnTargetDestroyed(AActor* DestroyedActor)
{
	AEnemyBase* enemy = Cast<AEnemyBase>(DestroyedActor);
	if (!enemy)
	{
		return;
	}
	//nie wiem czy potrzebne, ale chyba nie zaszkodzi
	enemy->OnDestroyed.RemoveDynamic(this, &ATowerBase::OnTargetDestroyed);
	if (_currentTarget && DestroyedActor == _currentTarget)
	{
		_currentTarget = nullptr;
	}
	_enemiesInRange.Remove(enemy);
}

void ATowerBase::AttackTarget(float deltaTime)
{
	//cd liczymy zawsze
	_timeSinceLastAttack += deltaTime;

	if (!EnsureHasTarget())
	{
		//nie mamy celu
		return;
	}

	if (!IsShootCooldownReady())
	{
		//dalej na cd
		return;
	}
	ShootProjectileAtTarget();
}

bool ATowerBase::IsShootCooldownReady()
{
	return _timeSinceLastAttack > (1 / AttackSpeed);
}

bool ATowerBase::EnsureHasTarget()
{
	bool hasTarget = false;
	if (!_currentTarget)
	{
		if (_enemiesInRange.Num() > 0)
		{
			_currentTarget = _enemiesInRange[0];
			hasTarget = true;
		}
	}
	else
	{
		hasTarget = true;
	}
	return hasTarget;
}

void ATowerBase::ShootProjectileAtTarget()
{
	if (!ProjectileClass || !_currentTarget)
	{
		return;
	}
	_timeSinceLastAttack = 0.f;

	FVector spawnLocation = GetActorLocation();
	FRotator spawnRotation = (_currentTarget->GetActorLocation() - GetActorLocation()).Rotation();
	FTransform spawnTransform(spawnRotation, spawnLocation);
	AProjectileBase* spawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(ProjectileClass, spawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (spawnedProjectile)
	{
		spawnedProjectile->SetTarget(_currentTarget);
		UGameplayStatics::FinishSpawningActor(spawnedProjectile, spawnTransform);
	}
}