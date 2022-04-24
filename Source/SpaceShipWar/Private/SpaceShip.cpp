// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShip.h"
//#include "Components/SphereComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Bullet.h"
#include "TimerManager.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
//#include "Public/Misc/App.h"// ��ȡdeltatime
//
// Sets default values
ASpaceShip::ASpaceShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bDead = false;
	// ʵ����mesh���
	ShipSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipSM"));
	// ָ������ĸ���������ûָ����Ĭ�ϵ�һ������Ǹ����
	// ʵ������ײ���
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	ThrusterPaticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ThrusterPaticle"));
	ThrusterPaticle->SetupAttachment(RootComponent);
	// ��mesh���ڸ������
	ShipSM->SetupAttachment(RootComponent);
	SpringArmComp= CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	// ָ���ӵ�������
	EmptyComp = CreateDefaultSubobject<USceneComponent>(TEXT("EmptyComp"));
	EmptyComp->SetupAttachment(ShipSM);
	speed = 2500;
	BetweenShot = 0.2f;

}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();
	// ��ȡ������
	PC = Cast<APlayerController>(GetController()) ;
	// ��ʾ���
	PC->bShowMouseCursor = true;
	
}

void ASpaceShip::LookAtCursor()
{
	FVector MouseLocation, MouseDirection;
	// ������λ��ת��Ϊ�������� ������
	PC->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	// ��ȡһ���Ƕ�,�ӵ�ǰλ�ÿ���Ŀ��λ���������ת�Ƕ�
	FVector TargetLocation = FVector(MouseLocation.X, MouseLocation.Y, GetActorLocation().Z);
	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	SetActorRotation(Rotator);
}

void ASpaceShip::MoveUp(float Value)
{
	if (Value != 0) {
		nUpMove = true;
	}
	else {
		nUpMove = false;
	}
	AddMovementInput(FVector::ForwardVector, Value);
}

void ASpaceShip::MoveRight(float Value)
{
	if (Value != 0) {
		nRightMove = true;
	}
	else {
		nRightMove = false;
	}
	AddMovementInput(FVector::RightVector, Value);
}

void ASpaceShip::Move(float DeltaTime)
{
	// ��ȡ�û����������ֵ
	AddActorWorldOffset(ConsumeMovementInputVector()* speed * DeltaTime,true) ;
}

void ASpaceShip::Fire()
{
	if (Bullet&&!bDead) {
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<ABullet>(Bullet, EmptyComp->GetComponentLocation(), EmptyComp->GetComponentRotation(), SpawnParams);
		if (ShootCue) {
			UGameplayStatics::PlaySoundAtLocation(this, ShootCue, GetActorLocation());
		}
		
	}
	
}

void ASpaceShip::StartFire()
{
	GetWorldTimerManager().SetTimer(TimerHandle_BetweenShot, this, &ASpaceShip::Fire, BetweenShot, true, 0.0f);
}

void ASpaceShip::EndFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BetweenShot);
}

void ASpaceShip::RestartLever()
{
	
	UGameplayStatics::OpenLevel(this, "MainMap");
}

void ASpaceShip::OnDeath()
{
	// ����Ҽ��� �ڶ��������Ǹ�����ɼ����Ƿ�Ӱ��������ɼ���
	CollisionComp->SetVisibility(false,true);
	bDead = true;
	
	if (GameOverCue) {
		UGameplayStatics::PlaySoundAtLocation(this, GameOverCue, GetActorLocation());
	}
	if (ExplosionPaticle) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionPaticle, GetActorLocation(), FRotator::ZeroRotator, true);
	}
	
	GetWorldTimerManager().SetTimer(TimerHandle_Restart, this, &ASpaceShip::RestartLever, 2.0f, false);
}

// Called every frame
void ASpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bDead) {
		if (nUpMove || nRightMove) {
			ThrusterPaticle->Activate();
		}
		else {
			ThrusterPaticle->Deactivate();
		}
		LookAtCursor();
		Move(DeltaTime);
	}

}

// Called to bind functionality to input
void ASpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// �������͵İ����룬һ������ӳ�䣨������������Ϊ�󶨣���һ�£�����ӳ��󶨵ĺ���һ��Ҫ����float����
	PlayerInputComponent->BindAxis("MoveUp",this,&ASpaceShip::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight",this,&ASpaceShip::MoveRight);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this,&ASpaceShip::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASpaceShip::EndFire);
}
void ASpaceShip::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy) {
		Enemy->Destroy();
		// ֱ���������ǿ��ܵ�����Ϸ����
		//Destroy();
		OnDeath();
		UE_LOG(LogTemp, Warning, TEXT("player is dead"));
	}
}

