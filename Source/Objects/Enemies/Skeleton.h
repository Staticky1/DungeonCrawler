#pragma once

#include "Objects/PawnObject.h"

//Skeleton Enemy, moves around and throws bones at the player

class Skeleton : public PawnObject
{
public:
	Skeleton();
	
	virtual void Tick(float DeltaTime) override;

	void ThrowBone(TVector2D<float> Direction);

	float MovementTimer = 0.f;

	//Randomises the current movement direction
	void RandomMovement();
	TVector2D<float> RandomMovementDirection;

	virtual void OnCollision(GameObject* OtherObject, bool bWasInstigator);

	float DistanceToPlayer = 999.f;

	float DamageCoolDown = 1.f;
	float DamageTimer = 0.f;
};