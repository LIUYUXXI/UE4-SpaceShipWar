// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawener.generated.h"

class AEnemy;
class UBoxComponent;
class ASpaceShip;
UCLASS()
class SPACESHIPWAR_API AEnemySpawener : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawener();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Enemy")
		TSubclassOf<AEnemy> Enemy;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		UBoxComponent* SpawnArea;
	FVector GetGenerartionLocation();

	float MinimumDistanceToPlayer = 1200.0f;
	ASpaceShip* SpaceShip;
	FTimerHandle TimerHandle_BetweenEnmeySpawn;
	float BetweenSpawn = 2.0f;
	void SpawnEnemy();

	UPROPERTY(EditAnywhere, Category = "Spawn")
	int MaxEnemyNum;

	int CurrentEnemyCount;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ReduceEnemyCount();
		
};
