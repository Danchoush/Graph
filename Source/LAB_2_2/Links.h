// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Links.generated.h"

UCLASS()
class LAB_2_2_API ALinks : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALinks();
	UPROPERTY()
		UInstancedStaticMeshComponent* LinkMesh;
	void CreateLink(FVector start, FVector finish, FLinearColor color, int radius);
};
