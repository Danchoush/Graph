// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Nodes.generated.h"

UCLASS()
class LAB_2_2_API ANodes : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANodes();
	TArray<FVector> Nodes;
	TArray<TArray<bool>> Links;

	UPROPERTY()
		UInstancedStaticMeshComponent* NodeMesh;

	void CreateLinks();
	void CreateRunners();
	void Init(int _Count_X, int _Count_Y, float _Radius, int _Runners_Count, float _Runners_Speed, UMaterial* AMaterial);
	FLinearColor GetContrastColor(int h);

	int Links_Count;
	int Count_X;
	int Count_Y;
	float Step;
	float Radius;
	int Runners_Count;
	float Runners_Speed;
	UMaterial* colMat;



private:

	void CreateGrid();
	int GetNeighbour(int CurrentNode);
	int GetUnstackedNeighbour(int CurrentNode, TArray<bool>& NodesMarks);

};
