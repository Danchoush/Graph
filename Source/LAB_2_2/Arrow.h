// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class LAB_2_2_API AArrow : public AActor
{
	GENERATED_BODY()

		UInstancedStaticMeshComponent* ArrowMesh;
	UInstancedStaticMeshComponent* HeadMesh;
public:
	
	AArrow();
	int Radius;
	UMaterialInstanceDynamic* Materia;
	

public:
	
	void CreateArrow(TArray<FVector> Path);
	void DestroyArrow();
};
