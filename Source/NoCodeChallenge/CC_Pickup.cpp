// Fill out your copyright notice in the Description page of Project Settings.

#include "CC_Pickup.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
ACC_Pickup::ACC_Pickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(SceneComponent);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComponent->AttachTo(SceneComponent);
}

// Called when the game starts or when spawned
void ACC_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACC_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

