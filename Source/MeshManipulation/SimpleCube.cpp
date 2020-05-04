#include "SimpleCube.h"
#include "ProceduralMeshComponent.h"	

// Sets default values
ASimpleCube::ASimpleCube()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralCubeMesh");
	/*Nessun tipo di collisione perchè non ci interessa nessun tipo di interazione*/
	CustomMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(CustomMesh);
}

/*Just a simple cube*/
void ASimpleCube::GenerateMesh(float X, float Y, float Z)
{
	Vertices.Add(FVector(0.f, 0.f, 0.f));
	Vertices.Add(FVector(X, 0.f, 0.f));
	Vertices.Add(FVector(X, Y, 0.f));
	Vertices.Add(FVector(0.f, Y, 0.f));
	Vertices.Add(FVector(0.f, 0.f, Z));
	Vertices.Add(FVector(X, 0.f, Z));
	Vertices.Add(FVector(X, Y, Z));
	Vertices.Add(FVector(0.f, Y, Z));

	//Bottom face
	AddTriangle(0, 1, 3);
	AddTriangle(1, 2, 3);

	//Top face
	AddTriangle(5, 4, 7);
	AddTriangle(5, 7, 6);

	//Forward Face 
	AddTriangle(5, 2, 1);
	AddTriangle(5, 6, 2);

	//Backward Face
	AddTriangle(4, 0, 3);
	AddTriangle(4, 3, 7);

	////Right Face
	AddTriangle(7, 3, 2);
	AddTriangle(7, 2, 6);

	////Left Face
	AddTriangle(4, 5, 1);
	AddTriangle(4, 1, 0);

	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	CustomMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void ASimpleCube::AddTriangle(int32 V1, int32 V2, int32 V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}

// Called when the game starts or when spawned
void ASimpleCube::BeginPlay()
{
	Super::BeginPlay();

}