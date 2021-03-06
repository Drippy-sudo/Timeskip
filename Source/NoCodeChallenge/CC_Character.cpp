// Fill out your copyright notice in the Description page of Project Settings.

#include "CC_Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
ACC_Character::ACC_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	SpringArm->TargetArmLength = 700.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Player Mesh"));
	MeshComp->SetupAttachment(RootComponent);

	TraceDistance = 2000.0f;

	PlayerRestRotation = GetActorRotation();
}

void ACC_Character::InteractPressed()
{
	TraceForward();
}

// Called when the game starts or when spawned
void ACC_Character::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ACC_Character::OnOverlapBegin);
}

void ACC_Character::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	IInteractInterface* Interface = Cast<IInteractInterface>(OtherActor);
	if (Interface)
	{
		Interface->InteractPure();
	}
}

void ACC_Character::TraceForward_Implementation()
{
	FVector Loc;
	FRotator Rot;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint(Loc, Rot);

	FVector Start = Loc;
	FVector End = Start + (Rot.Vector() * TraceDistance);

	FCollisionQueryParams TraceParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f);

	if (bHit)
	{
		IInteractInterface* Interface = Cast<IInteractInterface>(Hit.GetActor());
		if (Interface)
		{
			Interface->InteractPure(); // C++ version
		}
	}
}

void ACC_Character::SlowTime()
{
	static bool flip;
	static int count = 0;

	count++;
	if (count == 1)
		flip = true;
	else if (count == 2)
	{
		flip = false;
		count = 0;
	}	

	UE_LOG(LogTemp, Warning, TEXT("Count %i"), count);

	if (flip == true)
	{
		// TODO: do time since started slowing time
		// Make useable in code
		// Use for time UI

		// Slow global time dialation
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
		this->CustomTimeDilation = 10.0f;

		UE_LOG(LogTemp, Warning, TEXT("Time Slow"));

		IsTimeSlow = true;
	}
	else if (flip == false)
	{
		// Reset time dialation
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		this->CustomTimeDilation = 1.0f;

		UE_LOG(LogTemp, Warning, TEXT("Normal Time"));

		IsTimeSlow = false;
	}
}

// Called every frame
void ACC_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//float GlobalDeltaTimeSlowed = GetWorld()->GetTimeSeconds() / 10.0f;

	//UE_LOG(LogTemp, Warning, TEXT("GlobalDeltaTimeSlowed: %f"), GlobalDeltaTimeSlowed);
	//UE_LOG(LogTemp, Warning, TEXT("GetActorTimeDilation: %f"), GetActorTimeDilation());


	SetActorLocation(FVector(0.0f, GetActorLocation().Y, GetActorLocation().Z));	

	//if (GetActorTimeDilation == GlobalDeltaTimeSlowed)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Time is slow."));
	//	IsTimeSlow = true;
	//}
	//else
	//	IsTimeSlow = false;
}

// Called to bind functionality to input
void ACC_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("SlowTime", IE_Pressed, this, &ACC_Character::SlowTime);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACC_Character::InteractPressed);

	PlayerInputComponent->BindAxis("MoveRight", this, &ACC_Character::MoveRight);
}

void ACC_Character::MoveRight(float Value)
{
	if ((Controller) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		float rotationInput = 0.0f;
		static bool isFacingForward = true;

		if(IsTimeSlow == false)
			rotationInput = Direction.SizeSquared() * TurnSpeed * GetWorld()->GetDeltaSeconds();
		else if (IsTimeSlow == true)
		{
			rotationInput = Direction.SizeSquared() * TurnSpeed * GetWorld()->GetDeltaSeconds() * 10.0f;
			UE_LOG(LogTemp, Warning, TEXT("IsTimeSlow = true"));
		}

		if (GetActorRotation().Yaw > PlayerRestRotation.Yaw && Value == 1.0f)
		{
			isFacingForward = true;
			AddControllerYawInput(-rotationInput);
		}
		else if (GetActorRotation().Yaw < (PlayerRestRotation.Yaw + 175.0f) && Value == -1.0f)
		{
			isFacingForward = false;
			AddControllerYawInput(rotationInput);
		}
		else
		{
			AddControllerYawInput(0);
		}

		if (isFacingForward == true)
		{
			AddMovementInput(Direction, Value);
		}
		else
		{
			AddMovementInput(-Direction, Value);
		}
	}
}

void ACC_Character::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Implementation"));
}

void ACC_Character::InteractPure()
{
	UE_LOG(LogTemp, Warning, TEXT("Pure"));
}
