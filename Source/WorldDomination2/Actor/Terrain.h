// MIT License - Copyright (c) 2020 Oliver Knoll

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Terrain.generated.h"

class UProceduralMeshComponent;

UCLASS()
class WORLDDOMINATION2_API ATerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerrain();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UProceduralMeshComponent* TerrainMesh;

	// Mesh data
	TArray<FVector> Vertices;
	TArray<int32> Triangles;

	void AddTriangle(int32 V1, int32 V2, int32 V3);
	void GenerateCubeMesh();
	
};
