#pragma once

#include "GameCore/GameObject.h"
#include "GameCore/GameWorld.h"

struct FText
{
public: 
	char Character = ' ';
	TVector2D<int> Location = TVector2D<int>(0);
	TVector2D<int> Size = TVector2D<int>(0);

	FText() {};

	FText(char InChar, TVector2D<int> InLoc,TVector2D<int> InSize)
	{
		Character = InChar;
		Location = InLoc;
		Size = InSize;
	};
};

class TextObject : public GameObject
{
public:
	TextObject();

	TArray<FText> FontArray;

	FTileSheet* FontSprite = nullptr;

	void CreateText(TVector2D<float> ScreenLocation, std::string Text, int FontSize = 1);

	std::string DisplayText;
	TVector2D<float> Location;
	int FontSizeMod = 0;

	void Initalize(TVector2D<float> ScreenLocation, std::string Text, float TimeOnScreen, int FontSize = 1);

	virtual void Tick(float DeltaTime);

	float TimeUntilRemoval = 0.f;

protected:

	
};
