#pragma once

#include "PawnObject.h"

///
///
/// Player object is a pawn that can take input from the player
/// 
///

class PlayerObject : public PawnObject
{
public:
	PlayerObject();
	~PlayerObject();

	void Tick(float DeltaTime) override;

	virtual void HandleInput(float DeltaTime);

	void CheckSpriteRotation();

	virtual void OnAttack() override;

	virtual void OnCollision(GameObject* OtherObject, bool bWasInstigator) override;

	EDamageType AttackType = EDamageType::NORMAL;
};