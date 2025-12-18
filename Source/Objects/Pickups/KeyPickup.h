#pragma once

#include "GameCore/GameObject.h"

class KeyPickup : public GameObject
{
public:
	KeyPickup();

	virtual void OnCollision(GameObject* OtherObject, bool bWasInstigator) override;

	virtual void Tick(float DeltaTime) override;
};