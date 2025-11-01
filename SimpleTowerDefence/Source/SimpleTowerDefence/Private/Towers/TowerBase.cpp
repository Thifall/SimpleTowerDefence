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
	_enemiesInRange.Remove(enemy);
}

bool ATowerBase::IsAttackCooldownReady()
{
	return _timeSinceLastAttack > (1 / AttackSpeed);
}