// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceShip.generated.h"

class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class ABullet;
class USoundCue;
UCLASS()
class SPACESHIPWAR_API ASpaceShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// ��ʾ�ɻ�������
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* ShipSM;
	// ��ײ������
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USphereComponent* CollisionComp;
	// �������
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCameraComponent* CameraComp;
	//���ɱ����
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USceneComponent* EmptyComp;
	// ����ϵͳ���
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UParticleSystemComponent* ThrusterPaticle;
	// ��ը���Ӳ���Ҫ���ŷɴ��ߣ����Բ���Ҫ�������������
	UPROPERTY(EditAnywhere, Category = "Particle")
	UParticleSystem* ExplosionPaticle;

	// ��ȡplayercontroller �����������Ϊ�ɼ�
	APlayerController* PC;

	UPROPERTY(EditAnywhere, Category = "Fire")
	TSubclassOf<ABullet> Bullet;

	UPROPERTY(EditAnywhere, Category = "move")
	float speed;

	UPROPERTY(EditAnywhere, Category = "Fire")
	float BetweenShot;

	FTimerHandle TimerHandle_BetweenShot;
	FTimerHandle TimerHandle_Restart;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* GameOverCue;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* ShootCue;

	bool bDead;
	bool nUpMove;
	bool nRightMove;

	void LookAtCursor();
	void MoveUp(float);
	void MoveRight(float);
	void Move(float);
	void Fire();
	void StartFire();
	void EndFire();
	void RestartLever();
	void OnDeath();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	FORCEINLINE bool GetBDead() {
		return bDead;
	}

};
