// Copyright Anrility. All Rights Reserved.

#include "Props/RoadSpline.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

ARoadSpline::ARoadSpline()
{
	PrimaryActorTick.bCanEverTick = false;

	// Default root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Default Root"));
	Root->SetMobility(EComponentMobility::Static);

	SetRootComponent(Root);

	// Road Spline Component
	Road = CreateDefaultSubobject<USplineComponent>(TEXT("Road Spline"));
	Road->SetupAttachment(RootComponent);
	Road->SetMobility(EComponentMobility::Static);

	// Loading road asset
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RoadMeshObj(
		TEXT("/Script/Engine.StaticMesh'/Game/GameContent/Meshes/Static/SM_Road.SM_Road'")
	);

	RoadMesh = RoadMeshObj.Object;
}

// ==================== Lifecycles ==================== //

void ARoadSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Building the roads
	int32 Num = Road->GetNumberOfSplinePoints();

	for (int32 I = 0; I < Num - 1; ++I)
	{
		// Get the start and end position for each spline mesh
		FVector StartLocation, StartTangent;
		FVector EndLocation, EndTangent;

		Road->GetLocationAndTangentAtSplinePoint(I, StartLocation, StartTangent, ESplineCoordinateSpace::Local);
		Road->GetLocationAndTangentAtSplinePoint(I + 1, EndLocation, EndTangent, ESplineCoordinateSpace::Local);

		// Spline Mesh
		USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(
			AddComponentByClass(USplineMeshComponent::StaticClass(), false, FTransform(), false)
		);

		SplineMesh->SetMobility(EComponentMobility::Static);
		SplineMesh->SetStaticMesh(RoadMesh.LoadSynchronous());
		SplineMesh->SetForwardAxis(ESplineMeshAxis::X);
		SplineMesh->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent);
		SplineMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}
