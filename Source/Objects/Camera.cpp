#include "Camera.h"
#include "GameCore/GameWorld.h"
#include "Objects/PlayerObject.h"


Camera::Camera()
{
	bTicks = true;
}

Camera::~Camera()
{
}

void Camera::BeginPlay()
{
}

void Camera::Tick(float DeltaTime)
{
	TVector2D<float> MouseScreenSpaceLocation = TVector2D<float> (float(GameWorld::GetWorld()->GetMousePos().x), float (GameWorld::GetWorld()->GetMousePos().y));
	TVector2D<float> MouseWorldSpace = GameWorld::GetWorld()->ScreenPositionToWorldPosition(MouseScreenSpaceLocation);

	TVector2D<float> MouseAngle = (MouseWorldSpace - GameWorld::GetWorld()->Player->WorldPosition).GetNormalised();
	
	float DistanceToMouse = (MouseWorldSpace - GameWorld::GetWorld()->Player->WorldPosition).Magnitude();

	float Distance = DistanceToMouse < MaxDistance ? DistanceToMouse : MaxDistance;

	TVector2D<float> DesiredPos = (MouseAngle * Distance) + GameWorld::GetWorld()->Player->WorldPosition;

	WorldPosition = WorldPosition = WorldPosition.Lerp(DesiredPos, LerpSpeed * DeltaTime);

	/*
	TVector2D<float> DesiredPos = MouseWorldSpace.Lerp(GameWorld::GetWorld()->Player->WorldPosition, 0.5f);

	float Distance = (WorldPosition - DesiredPos).Magnitude();
	float PercentToMove = (LerpSpeed * DeltaTime) / Distance;

	
	*/
}
