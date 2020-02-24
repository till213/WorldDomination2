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
	// Actor local coordinates
	Vertices.Add(FVector(100, -100, 0)); // bottom-left
	Vertices.Add(FVector(-100, -100, 0)); // bottom-right
	Vertices.Add(FVector(-100, 100, 0)); // top-right
	Vertices.Add(FVector(100, 100, 0)); // top-left
	
	// Rectangle
	AddTriangle(0, 1, 2);
	AddTriangle(0, 2, 3);

	TArray<FLinearColor> VertexColors;
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	
	TerrainMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(),true);
	// Enable collision data
	TerrainMesh->ContainsPhysicsTriMeshData(true);	
}
