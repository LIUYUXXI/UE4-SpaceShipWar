// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

class ASpaceShip;
class USphereComponent;
class AShipGameMode;
class AEnemySpawener;
UCLASS()
class SPACESHIPWAR_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// 显示飞机的样子
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* ShipSM;
	// 碰撞体积组件
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USphereComponent* CollisionComp;
	// 运动组件 不能使用
	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//UProjectileMovementComponent* ProjectileMoveComp;

	ASpaceShip* SpaceShip;

	float Speed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Particle")
	UParticleSystem* ExplosionPaticle;

	void MoveTowardsPlayer(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
	void SetColor();
	

	AShipGameMode* MyGameMode;
	AEnemySpawener* EnemySpawner;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnDeath();
};
