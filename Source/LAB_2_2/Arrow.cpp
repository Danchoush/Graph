// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"


AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;


	ArrowMesh =
		CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedArrowMesh"));
	ArrowMesh->AttachToComponent(GetRootComponent(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> planeMesh(
		TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	ArrowMesh->SetStaticMesh(planeMesh.Object);
	ArrowMesh->SetCastShadow(false);


	HeadMesh =
		CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedHeadMesh"));
	HeadMesh->AttachToComponent(GetRootComponent(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> arrMesh(
		TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'"));
	HeadMesh->SetStaticMesh(arrMesh.Object);
	HeadMesh->SetCastShadow(false);

}

void AArrow::CreateArrow(TArray<FVector> Path)
{
	HeadMesh->SetMaterial(0, Materia);
	ArrowMesh->SetMaterial(0, Materia);
	for (int x = 0; x < Path.Num() - 1; x++) {
		FVector start = Path[x];
		FVector finish = Path[x + 1];
		float width = FVector::Distance(start, finish) / 100;
		float height = 0.4 * Radius;

		FVector location(start.X + (finish.X - start.X) / 2, start.Y + (finish.Y - start.Y) / 2, start.Z + (finish.Z - start.Z) / 2);
		FRotator rotation = FVector(finish - start).Rotation();
		rotation.Pitch -= 90;
		FVector location_head(location.X + (finish.X - location.X) / 2, location.Y + (finish.Y - location.Y) / 2, location.Z + (finish.Z - location.Z) / 2);

		ArrowMesh->AddInstance(FTransform(rotation, location, FVector(height, height, width)));
		HeadMesh->AddInstance(FTransform(rotation, location_head, FVector(height * 3, height * 3, height * 3)));
	}
}

void AArrow::DestroyArrow()
{
	ArrowMesh->RemoveInstance(0);
	HeadMesh->RemoveInstance(0);
}



