// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
// ��ȡ����spaceship
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
	// ʵ����mesh���
	ShipSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipSM"));
	// ָ������ĸ���������ûָ����Ĭ�ϵ�һ������Ǹ����
	// ʵ������ײ���
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	// ��mesh���ڸ������
	ShipSM->SetupAttachment(RootComponent);
	// �˶���������ڸ�����ϣ������������һ��
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

