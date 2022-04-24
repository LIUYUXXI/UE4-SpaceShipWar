// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy.h"
#include"Engine/BlockingVolume.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BulletSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletSM"));
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;
	BulletSM->SetupAttachment(RootComponent);
	// 运动组件不绑定在根组件上，跟根组件并列一级
	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComp"));

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// 相撞的两个物体中至少有一个物体的碰撞预设设为overlap
void ABullet::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	AEnemy* Enemy =  Cast<AEnemy>(OtherActor);
	if (Enemy) {
		
		Enemy->OnDeath();
		Destroy();
	}
	else if (Cast<ABlockingVolume>(OtherActor)) {
		Destroy();
	}
}

