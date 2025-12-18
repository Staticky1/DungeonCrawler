#pragma once

#include "GameCore/GameObject.h"

enum class ETileType
{
	WALL,
	WALL_CORNER_LEFT,
	WALL_CORNER_RIGHT,
	TILEFLOOR,
	SANDFLOOR,
	CEILING,
	DECOR,
};

class BackgroundTile : public GameObject
{
public:

	BackgroundTile();
	~BackgroundTile();

	void SetTileType(ETileType NewType);

	ETileType TileType = ETileType::TILEFLOOR;

};