// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
// 获取主角spaceship
#include "Kismet/GameplayStatics.h"
#include "SpaceShip.h"
#include "ShipGameMode.h"
#include "EnemySpawener.h"
#include "Particles/ParticleSystem.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 实例化mesh组件
	ShipSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipSM"));
	// 指定物体的根组件，如果没指定，默认第一个组件是根组件
	// 实例化碰撞组件
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	// 将mesh绑定在根组件上
	ShipSM->SetupAttachment(RootComponent);
	// 运动组件不绑定在根组件上，跟根组件并列一级
	//ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComp"));

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	MyGameMode= Cast<AShipGameMode> (UGameplayStatics::GetGameMode(this));
	SpaceShip= Cast<ASpaceShip>(UGameplayStatics::GetPlayerPawn(this, 0));
	SetColor();
	TArray<AActor*> EnemySpawnArray;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemySpawener::StaticClass(), EnemySpawnArray);
	EnemySpawner = Cast<AEnemySpawener>(EnemySpawnArray[0]);
	
}

void AEnemy::MoveTowardsPlayer(float DeltaTime)
{	
	
	//FVector TargetLocation = FVector(SpaceShip->GetActorLocation().X, SpaceShip->GetActorLocation().Y, GetActorLocation().Z);
	FVector Direction = FVector(SpaceShip->GetActorLocation()-GetActorLocation()).GetSafeNormal();
	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), SpaceShip->GetActorLocation());
	SetActorRotation(Rotator);
	AddActorWorldOffset(Direction*Speed*DeltaTime,true);
}

void AEnemy::OnDeath()
{
	MyGameMode->IncreaseScore();
	EnemySpawner->ReduceEnemyCount();
	if (ExplosionPaticle) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionPaticle, GetActorLocation(), FRotator::ZeroRotator, true);


	}
	
	Destroy();
}


// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SpaceShip->GetBDead()==false) {
		MoveTowardsPlayer(DeltaTime);
	}
	

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

