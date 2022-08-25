// Fill out your copyright notice in the Description page of Project Settings.


#include "BTItemBox.h"
#include "BTCharacter.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/EngineTypes.h"
#include "BTWeapon.h"

// Sets default values
ABTItemBox::ABTItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
    Trigger->SetCollisionProfileName(TEXT("ItemBox"));
    RootComponent = Trigger;
    Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));


    Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
    Box->SetCollisionProfileName(TEXT("NoCollision"));
    Box->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
    if (SM_BOX.Succeeded())
    {
        Box->SetStaticMesh(SM_BOX.Object);
    }
    Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

    WeaponItemClass = ABTWeapon::StaticClass();

    Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
    Effect->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
    if (P_CHESTOPEN.Succeeded())
    {
        Effect->SetTemplate(P_CHESTOPEN.Object);
        Effect->bAutoActivate = false;
    }

}

// Called when the game starts or when spawned
void ABTItemBox::BeginPlay()
{
	Super::BeginPlay();


	
}

void ABTItemBox::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ABTItemBox::OnCharacterOverlap);

}


// Called every frame
void ABTItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABTItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("Item eat"));
    auto ABCharacter = Cast<ABTCharacter>(OtherActor);

    if (nullptr != ABCharacter && nullptr != WeaponItemClass)
    {
        if (ABCharacter->CanSetWeapon())
        {
            auto NewWeapon = GetWorld()->SpawnActor<ABTWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
            ABCharacter->SetWeapon(NewWeapon);
            Effect->Activate(true);
            Box->SetHiddenInGame(true, true);
            SetActorEnableCollision(false);
            Effect->OnSystemFinished.AddDynamic(this, &ABTItemBox::OnEffectFinished);
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("%s can't equip weapon currently."), *ABCharacter->GetName());
        }
    }
}

void ABTItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
    Destroy();
}

