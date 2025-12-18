#pragma once

#include "GameCore/GameObject.h"

// Object that when the player colliedes with it displays a game win message

class WinTrigger : public GameObject
{
public:	
	WinTrigger();

	virtual void OnCollision(GameObject* OtherObject, bool bWasInstigator) override;


};