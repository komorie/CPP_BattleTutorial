// Fill out your copyright notice in the Description page of Project Settings.


#include "Fountain.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/PointLightComponent.h"

// Sets default values
AFountain::AFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WATER"));
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"));
	Splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SPLASH"));


	RootComponent = Body;
	Water->SetupAttachment(Body);
	//Light->SetupAttachment(Body);
	Splash->SetupAttachment(Body);


	Water->SetRelativeLocation(FVector(0.f, 0.f, 135.f));
	//Light->SetRelativeLocation(FVector(0.f, 0.f, 135.f));
	Splash->SetRelativeLocation(FVector(0.f, 0.f, 195.f));


	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_BODY(TEXT("/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01"));
	if (SM_BODY.Succeeded())
	{
		Body->SetStaticMesh(SM_BODY.Object);
	}
}

// Called when the game starts or when spawned
void AFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

