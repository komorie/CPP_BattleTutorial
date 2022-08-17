// Fill out your copyright notice in the Description page of Project Settings.


#include "BTWeapon.h"

// Sets default values
ABTWeapon::ABTWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackWyrmBlade/SK_Blade_BlackWyrmBlade.SK_Blade_BlackWyrmBlade'"));

	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void ABTWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABTWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

