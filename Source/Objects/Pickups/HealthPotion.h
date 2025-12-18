#pragma once

#include "GameCore/GameObject.h"

class HealthPotion : public GameObject
{
public:
	HealthPotion();

	virtual void OnCollision(GameObject* OtherObject, bool bWasInstigator) override;

	virtual void Tick(float DeltaTime) override;
};