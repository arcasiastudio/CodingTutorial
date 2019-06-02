// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPawn.h"
#include "Classes/Components/InputComponent.h"
#include "Classes/GameFramework/FloatingPawnMovement.h"
#include "Classes/GameFramework/SpringArmComponent.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Classes/Camera/CameraComponent.h"

#include "Bullet.h"

// Sets default values
ACustomPawn::ACustomPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovement");
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

	//For Spring Arm
	CameraArm = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArm");
	CameraArm->SetupAttachment(StaticMesh);
	CameraArm->TargetArmLength = 500.f;

	Camera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");

	//Camera->SetRelativeLocation(FVector(-500.f, 0, 0));
	Camera->SetupAttachment(CameraArm);

	//Must have root component
	SetRootComponent(StaticMesh);

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;

	
}
// Called when the game starts or when spawned
void ACustomPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACustomPawn::MoveForward(float Amount)
{
	FloatingPawnMovement->AddInputVector(GetActorForwardVector() * Amount);
}

void ACustomPawn::MoveRight(float Amount)
{
	FloatingPawnMovement->AddInputVector(GetActorRightVector() * Amount);
}

void ACustomPawn::Turn(float Amount)
{
	AddControllerYawInput(Amount);
}

void ACustomPawn::LookUp(float Amount)
{
	AddControllerPitchInput(Amount);
}

void ACustomPawn::Shoot()
{
	if (BulletClass != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.bNoFail = true;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		//Transform for Bullet
		FTransform BulletSpawnTransform;
		BulletSpawnTransform.SetLocation(GetActorForwardVector() * 500.f + GetActorLocation());
		BulletSpawnTransform.SetRotation(GetActorRotation().Quaternion());
		BulletSpawnTransform.SetScale3D(FVector(1.f));

		GetWorld()->SpawnActor<ABullet>(BulletClass, BulletSpawnTransform, SpawnParams);
	}
	
}

// Called every frame
void ACustomPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACustomPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACustomPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACustomPawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACustomPawn::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ACustomPawn::LookUp);

	//Shoot input
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACustomPawn::Shoot);
}

