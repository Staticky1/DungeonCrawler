#pragma once

#include "GameCore/GameObject.h"

class PlayerObject;

class HealthDisplay : public GameObject
{
public:
	HealthDisplay();

	virtual void Tick(float DeltaTime) override;

	//sets vars for the display
	void InitalizeDisplay(PlayerObject* InPlayer, TVector2D<int> ScreenPos);

	PlayerObject* Player = nullptr;

	//Draws the display to the screen with new stats from the player pointer
	void UpdateDisplay();

	TVector2D<int> ScreenLocation;

	FTileSheet* SpriteSheet = nullptr;

	TVector2D<int> FullHeartOffset;
	TVector2D<int> HalfHeartOffset;
	TVector2D<int> EmptyHeartOffset;
};