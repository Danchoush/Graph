// Fill out your copyright notice in the Description page of Project Settings.


#include "Links.h"
#include "Nodes.h"


// Sets default values
ALinks::ALinks()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	LinkMesh =
		CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedLinkMesh"));
	LinkMesh->AttachToComponent(GetRootComponent(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> planeMesh(
		TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	LinkMesh->SetStaticMesh(planeMesh.Object);
	LinkMesh->SetCastShadow(false);
}


void ALinks::CreateLink(FVector start, FVector finish, FLinearColor color, int radius)
{
	const int32 instancedIndex = LinkMesh->GetInstanceCount();

	float width = FVector::Distance(start, finish) / 100;
	float height = 0.2 * radius;

	FVector location(start.X + (finish.X - start.X) / 2, start.Y + (finish.Y - start.Y) / 2, start.Z + (finish.Z - start.Z) / 2);
	FRotator rotation = FVector(finish - start).Rotation();
	rotation.Pitch += 90;

	LinkMesh->AddInstance(FTransform(rotation, location, FVector(height, height, width)));


}

