// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawener.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
// 获取主角spaceship
#include "Kismet/GameplayStatics.h"
#include "SpaceShip.h"
// 生成物体
#include "Engine/World.h"
#include "TimerManager.h"
// Sets default values
AEnemySpawener::AEnemySpawener()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;
	MaxEnemyNum = 30;
	CurrentEnemyCount = 0;

}

void AEnemySpawener::BeginPlay()
{
	Super::BeginPlay();
	SpaceShip = Cast<ASpaceShip>(UGameplayStatics::GetPlayerPawn(this, 0));
	GetWorldTimerManager().SetTimer(TimerHandle_BetweenEnmeySpawn, this, &AEnemySpawener::SpawnEnemy, BetweenSpawn, true, 0.0f);

}
FVector AEnemySpawener::GetGenerartionLocation()
{
	// 生成随机点
	float Distance = 0.0f;
	FVector Location;
	while (Distance < MinimumDistanceToPlayer) {
		Location = UKismetMathLibrary::RandomPointInBoundingBox(SpawnArea->Bounds.Origin, SpawnArea->Bounds.BoxExtent);
		Distance = (Location - SpaceShip->GetActorLocation()).Size();
	}
	return Location;
}

void AEnemySpawener::SpawnEnemy()
{
	if (Enemy&&SpaceShip->GetBDead()==false && CurrentEnemyCount<MaxEnemyNum) {
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AEnemy>(Enemy, GetGenerartionLocation(), FRotator::ZeroRotator, SpawnParams);
		++CurrentEnemyCount;
	}
}

// Called when the game starts or when spawned

// Called every frame
void AEnemySpawener::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawener::ReduceEnemyCount()
{
	if (CurrentEnemyCount > 0) {
		--CurrentEnemyCount;
	}
}

