// Fill out your copyright notice in the Description page of Project Settings.


#include "BTSection.h"
#include "Components/BoxComponent.h"

// Sets default values


//섹션의 배경과 네 방향으로 캐릭터의 입장을 통제하는 문을 제공한다.
//플레이어가 섹션에 진입하면 모든 문을 닫는다.
//문을 닫고 일정 시간 후에 섹션 중앙에 NPC를 생성한다.
//문을 닫고 일정 시간 후에 아이템 상자를 섹션 내 랜덤한 위치에 생성한다.
//생성한 NPC가 죽으면 모든 문을 개방한다.
//통과한 문으로 이어지는 새로운 섹션을 생성한다.

ABTSection::ABTSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 액터 루트 컴포넌트로 메쉬 생성, 설정
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	//문 메쉬 불러오기
	FString GateAssetPath = TEXT("StaticMesh'/Game/Book/StaticMesh/SM_GATE.SM_GATE'");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*GateAssetPath);

	//경기장 메쉬 불러오기, 메쉬 컴포넌트에 경기장 메쉬 지정
	FString AssetPath = TEXT("StaticMesh'/Game/Book/StaticMesh/SM_SQUARE.SM_SQUARE'");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUARE(*AssetPath);
	if (SM_SQUARE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_SQUARE.Object);
	}

	//트리거 생성, 모양 설정, 부착. 
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Trigger->SetCollisionProfileName(TEXT("BTTrigger"));

	//트리거에 콜리전이 일어나면 델리게이트로 nTriggerBeginOverlap 함수 실행
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ABTSection::OnTriggerBeginOverlap);


}

void ABTSection::OnConstruction(const FTransform& Transform)
{
}

// Called when the game starts or when spawned
void ABTSection::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABTSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABTSection::SetState(ESectionState NewState)
{
}

void ABTSection::OperateGates(bool bOpen)
{
}

void ABTSection::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABTSection::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABTSection::OnNPCSpawn()
{
}

