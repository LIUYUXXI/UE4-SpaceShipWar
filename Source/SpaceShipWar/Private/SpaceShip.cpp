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
//#include "Public/Misc/App.h"// 获取deltatime
//
// Sets default values
ASpaceShip::ASpaceShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bDead = false;
	// 实例化mesh组件
	ShipSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipSM"));
	// 指定物体的根组件，如果没指定，默认第一个组件是根组件
	// 实例化碰撞组件
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	ThrusterPaticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ThrusterPaticle"));
	ThrusterPaticle->SetupAttachment(RootComponent);
	// 将mesh绑定在根组件上
	ShipSM->SetupAttachment(RootComponent);
	SpringArmComp= CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	// 指定子弹出生点
	EmptyComp = CreateDefaultSubobject<USceneComponent>(TEXT("EmptyComp"));
	EmptyComp->SetupAttachment(ShipSM);
	speed = 2500;
	BetweenShot = 0.2f;

}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();
	// 获取控制器
	PC = Cast<APlayerController>(GetController()) ;
	// 显示鼠标
	PC->bShowMouseCursor = true;
	
}

void ASpaceShip::LookAtCursor()
{
	FVector MouseLocation, MouseDirection;
	// 将鼠标的位置转化为世界坐标 ？？？
	PC->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	// 获取一个角度,从当前位置看向目标位置所需的旋转角度
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
	// 获取用户输入的向量值
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
	// 让玩家假死 第二个参数是该组件可见性是否影响子组件可见性
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
	// 两种类型的绑定输入，一种是轴映射（持续按），行为绑定（按一下），轴映射绑定的函数一定要传入float参数
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
		// 直接销毁主角可能导致游戏崩溃
		//Destroy();
		OnDeath();
		UE_LOG(LogTemp, Warning, TEXT("player is dead"));
	}
}

