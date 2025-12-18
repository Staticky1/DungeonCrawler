#pragma once

#include "Objects/PawnObject.h"

//Snake enemy class

class Snake : public PawnObject
{

public:
	Snake();

	virtual void Tick(float DeltaTime) override;

	float LungeSpeed = 0.f;
	float LungeRange = 80.f;
	float KnockbackForce = 100.f;

	void LungeAttack(TVector2D<float> Direction);

	float MovementTimer = 0.f;

	//Randomises the current movement direction
	void RandomMovement();
	TVector2D<float> RandomMovementDirection;

	virtual void OnCollision(GameObject* OtherObject, bool bWasInstigator);

	float DistanceToPlayer = 999.f;

	float DamageCoolDown = 1.f;
	float DamageTimer = 0.f;

	float AtkDamageCoolDown = 0.5f;
	float AtkDamageTimer = 0.f;
};