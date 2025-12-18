#pragma once

#include "Math.h"
#include "TArray.h"
#include "TVector2D.h"
#include "olcPixelGameEngine.h"

#include <iostream>

///
/// A collection of misc enums and structs that are applicable to a number of classes
/// 

class GameObject;

enum class ETickGroup
{
	WORKGROUP, //Main tick loop
	PREWORK, //called before main loop
	POSTWORK, //called after main loop
};

//Struct class containing a tileset
struct FTileSheet
{

private:

	olc::Sprite* TileSheetPointer = nullptr;

public:

	std::string TileSetpath = "";

	FTileSheet()
	{
	};

	FTileSheet(const std::string& InPath)
	{
		TileSetpath = InPath;
		TileSheetPointer = new olc::Sprite(InPath);

	}

	olc::Sprite* Get() const
	{
		return TileSheetPointer;
	}

	void SetPath(const std::string& InPath)
	{
		TileSetpath = InPath;
		TileSheetPointer = new olc::Sprite(InPath);
	}

	~FTileSheet()
	{
		if (TileSheetPointer)
		{
			delete TileSheetPointer;
		}
	};

};

enum class EAnimationType
{
	NONE, //Default Value
	STILL, //No Animation is played
	LOOP_ONCE, //Plays through the animation once then stops on the last frame
	LOOP_FOREVER, //loops the animation
};

//Struct for a basic non-animated sprite
struct FSprite
{

public:

	//Refrence to the tilesheet containing the sprite
	const FTileSheet* TileSheet = nullptr;

	//The location of the sprites top left corner on the tilesheet
	TVector2D<int> SpriteOffset = TVector2D<int>(0, 0);

	//Size of the sprite on the tilesheet
	TVector2D<int> SpriteSize = TVector2D<int>(0, 0);

	//Tint to apply to the sprite R,G,B,A from 0-1;
	TArray<float> SpriteTint = TArray<float>({ 0.f,0.f,0.f,1.f });

	//Is the sprite animated
	EAnimationType AnimationType = EAnimationType::NONE;

	//number of frames for animation
	int NumberOfFrames = 0;

	//lenght of the animation in seconds
	float AnimationLength = 0;

	bool bIsAnimationPaused = false;

	//Time the animation has been playing, resets to zero if the animation restarts
	float AnimationPlayTime = 0.f;

	//Passed to the draw function to control the flip of the sprite, 0 = None 1 = Horiz 2 = vert
	int FlipSprite = 1;

	//Does the sprite require alpha or a mask
	olc::Pixel::Mode RequiredPixelMode = olc::Pixel::NORMAL;

	FSprite()
	{
	}

	FSprite(const FTileSheet* InTileSheet, TVector2D<int> InOffset, TVector2D<int> InSize)
	{
		TileSheet = InTileSheet;
		SpriteOffset = InOffset;
		SpriteSize = InSize;
	}

	TVector2D<int> CalculateFramePosition(int FrameNumber = 0)
	{
		switch (AnimationType)
		{
		case EAnimationType::NONE:
			return SpriteOffset;
			break;
		case EAnimationType::STILL:
			return SpriteOffset;
			break;
		case EAnimationType::LOOP_ONCE:
			return TVector2D<int>(SpriteOffset.X + (SpriteSize.X * FrameNumber), SpriteOffset.Y);
			break;
		case EAnimationType::LOOP_FOREVER:
			return TVector2D<int>(SpriteOffset.X + (SpriteSize.X * FrameNumber), SpriteOffset.Y);
			break;
		default:
			return SpriteOffset;
			break;
		}		
	}

};

enum class EHitBoxShape
{
	SQUARE,
	CIRCLE,
};

//Contains properties used for checking collisions
struct FHitBox
{

public:

	TVector2D<float> Size;
	EHitBoxShape Shape;

	TArray<GameObject*> IgnoredObjects;

	TVector2D<float> OffsetFromOrign;

	//Draw hit box ingame for debuging
	bool bShowHitBoxInGame = false;

	FHitBox()
		:Size(TVector2D<float>(0.f))
		,Shape(EHitBoxShape::SQUARE)
		,OffsetFromOrign(TVector2D<float>(0.f))
	{};
};