// MIT License - Copyright (c) 2020 Oliver Knoll

#include "Terrain.h"
#include <ProceduralMeshComponent.h>

// Public

// Sets default values
ATerrain::ATerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TerrainMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Terrain");
	SetRootComponent(TerrainMesh);
	TerrainMesh->bUseAsyncCooking = true;
}

// Called every frame
void ATerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Protected

// / This is called when actor is spawned (at runtime or when you drop it into the world in editor)
void ATerrain::PostActorCreated()
{
	Super::PostActorCreated();
	GenerateCubeMesh();
}

// This is called when actor is already in level and map is opened
void ATerrain::PostLoad()
{
	Super::PostLoad();
	GenerateCubeMesh();
}

// Called when the game starts or when spawned
void ATerrain::BeginPlay()
{
	Super::BeginPlay();
}

void ATerrain::AddTriangle(int32 V1, int32 V2, int32 V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}

void ATerrain::GenerateCubeMesh()
{
	// 6 sides on cube, 4 verts each (corners)
	
	// These are relative locations to the placed Actor in the world
	Vertices.Add(FVector(0, -100, 0)); // lower left - 0
	Vertices.Add(FVector(0, -100, 100)); // upper left - 1
	Vertices.Add(FVector(0, 100, 0)); // lower right - 2 
	Vertices.Add(FVector(0, 100, 100)); // upper right - 3
	
	Vertices.Add(FVector(100, -100, 0)); // lower front left - 4
	Vertices.Add(FVector(100, -100, 100)); // upper front left - 5

	Vertices.Add(FVector(100, 100, 100)); // upper front right - 6
	Vertices.Add(FVector(100, 100, 0)); // lower front right - 7

	// Back face of cube
	AddTriangle(0, 2, 3);
	AddTriangle(3, 1, 0);

	// Left face of cube
	AddTriangle(0, 1, 4);
	AddTriangle(4, 1, 5);

	// Front face of cube
	AddTriangle(4, 5, 7);
	AddTriangle(7, 5, 6);

	// Right face of cube
	AddTriangle(7, 6, 3);
	AddTriangle(3, 2, 7);

	// Top face
	AddTriangle(1, 3, 5);
	AddTriangle(6, 5, 3);

	// bottom face
	AddTriangle(2, 0, 4);
	AddTriangle(4, 7, 2);

	TArray<FLinearColor> VertexColors;
	VertexColors.Add(FLinearColor(0.f, 0.f, 1.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.5f, 1.f, 0.5f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 1.f));

	TerrainMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(),true);
	// Enable collision data
	// TerrainMesh->ContainsPhysicsTriMeshData(true);
}
