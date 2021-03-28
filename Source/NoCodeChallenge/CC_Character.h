// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractInterface.h"

#include "CC_Character.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class NOCODECHALLENGE_API ACC_Character : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACC_Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Mesh")
	UStaticMeshComponent* MeshComp;

	void InteractPressed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float TraceDistance;

	UFUNCTION(BlueprintNativeEvent)
	void TraceForward();
	void TraceForward_Implementation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Movement functions
	void MoveRight(float Value);

	void SlowTime();

	void Interact_Implementation() override; // Blueprint version

	virtual void InteractPure() override; // C++ version

	FRotator PlayerRestRotation;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnSpeed = 150.0f;

	bool IsTimeSlow = false;
	bool IsTickOff = false;
};
