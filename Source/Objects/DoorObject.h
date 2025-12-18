#pragma once

#include "GameCore/GameObject.h"


class DoorObject : public GameObject
{
	FSprite OpenSprite;
	FSprite ClosedSprite;

	TVector2D<float> OpenSpriteOffset;
	TVector2D<float> ClosedSpriteOffset;

	int DoorDirectionVar = 0;

	float DisplayTextTimer = 0.f;
	float DisplayTextCoolDown = 2.f;
	bool bCanDisplayText = true;

public:
	DoorObject();

	//Initalised the door
	void CreateDoor(int DoorDirection, bool bStartLocked);

	//checks to see if door can open and opens door
	void OpenDoor();

	bool bIsLocked = false;

	bool bIsOpen = false;

	void Tick(float DeltaTime) override;
};