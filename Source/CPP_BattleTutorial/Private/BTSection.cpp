// Fill out your copyright notice in the Description page of Project Settings.


#include "BTSection.h"
#include "Components/BoxComponent.h"

// Sets default values


//������ ���� �� �������� ĳ������ ������ �����ϴ� ���� �����Ѵ�.
//�÷��̾ ���ǿ� �����ϸ� ��� ���� �ݴ´�.
//���� �ݰ� ���� �ð� �Ŀ� ���� �߾ӿ� NPC�� �����Ѵ�.
//���� �ݰ� ���� �ð� �Ŀ� ������ ���ڸ� ���� �� ������ ��ġ�� �����Ѵ�.
//������ NPC�� ������ ��� ���� �����Ѵ�.
//����� ������ �̾����� ���ο� ������ �����Ѵ�.

ABTSection::ABTSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// ���� ��Ʈ ������Ʈ�� �޽� ����, ����
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	//�� �޽� �ҷ�����
	FString GateAssetPath = TEXT("StaticMesh'/Game/Book/StaticMesh/SM_GATE.SM_GATE'");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*GateAssetPath);

	//����� �޽� �ҷ�����, �޽� ������Ʈ�� ����� �޽� ����
	FString AssetPath = TEXT("StaticMesh'/Game/Book/StaticMesh/SM_SQUARE.SM_SQUARE'");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUARE(*AssetPath);
	if (SM_SQUARE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_SQUARE.Object);
	}

	//Ʈ���� ����, ��� ����, ����. 
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Trigger->SetCollisionProfileName(TEXT("BTTrigger"));

	//Ʈ���ſ� �ݸ����� �Ͼ�� ��������Ʈ�� nTriggerBeginOverlap �Լ� ����
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

