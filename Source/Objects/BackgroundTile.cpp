#include "BackgroundTile.h"
#include "GameCore/GameWorld.h"

BackgroundTile::BackgroundTile()
{
	bShouldDraw = true;
	bTicks = false;

	Sprite.TileSheet = GameWorld::GetWorld()->BackgroundTileSheet;
	
	ObjectHitBox.bShowHitBoxInGame = false;

}

BackgroundTile::~BackgroundTile()
{
}

void BackgroundTile::SetTileType(ETileType NewType)
{
	TileType = NewType;

	switch (NewType)
	{
	case ETileType::WALL:
	{
		TArray<TVector2D<int>> WallSprites;
		WallSprites.AddElement(TVector2D<int>(16, 16));
		WallSprites.AddElement(TVector2D<int>(32, 16));
		WallSprites.AddElement(TVector2D<int>(48, 16));
		WallSprites.AddElement(TVector2D<int>(64, 16));

		Sprite.SpriteOffset = WallSprites.RandomElement();

		SpriteOffset = TVector2D<float>(8, 16);

		Sprite.SpriteSize = TVector2D<int>(16, 32);

		ObjectHitBox.Size = TVector2D<float>(16);
		ObjectHitBox.OffsetFromOrign = TVector2D<float>(8,0);
		bHasCollision = true;

		Sprite.FlipSprite = std::rand() % 2;

		break;
	}
	case ETileType::WALL_CORNER_LEFT:
	{
		Sprite.SpriteOffset = TVector2D<int>(0, 16);
		Sprite.SpriteSize = TVector2D<int>(16, 32);

		SpriteOffset = TVector2D<float>(8, 16);


		ObjectHitBox.Size = TVector2D<float>(16);
		ObjectHitBox.OffsetFromOrign = TVector2D<float>(8, 0);
		bHasCollision = true;

		break;
	}
	case ETileType::WALL_CORNER_RIGHT:
	{
		Sprite.SpriteOffset = TVector2D<int>(80, 16);
		Sprite.SpriteSize = TVector2D<int>(16, 32);

		SpriteOffset = TVector2D<float>(8, 16);


		ObjectHitBox.Size = TVector2D<float>(16);
		ObjectHitBox.OffsetFromOrign = TVector2D<float>(8, 0);
		bHasCollision = true;

		break;
	}
	case ETileType::TILEFLOOR:
	{
		TArray<TVector2D<int>> FoorSprites;
		FoorSprites.AddElement(TVector2D<int>(144, 96));
		FoorSprites.AddElement(TVector2D<int>(160, 96));
		FoorSprites.AddElement(TVector2D<int>(176, 96));

		Sprite.SpriteOffset = FoorSprites.RandomElement();

		SpriteOffset = TVector2D<float>(8);

		ObjectZHeight = -2;

		Sprite.SpriteSize = TVector2D<int>(16);
		break;
	}
	case ETileType::SANDFLOOR:
	{
		TArray<TVector2D<int>> FoorSprites;
		FoorSprites.AddElement(TVector2D<int>(0, 64)); //Adds blank sandy space more times so it's more likely to come up in random selection
		FoorSprites.AddElement(TVector2D<int>(0, 64));
		FoorSprites.AddElement(TVector2D<int>(0, 64));
		FoorSprites.AddElement(TVector2D<int>(0, 64));
		FoorSprites.AddElement(TVector2D<int>(0, 64));
		FoorSprites.AddElement(TVector2D<int>(16, 96));
		FoorSprites.AddElement(TVector2D<int>(32, 96));
		FoorSprites.AddElement(TVector2D<int>(48, 96));
		FoorSprites.AddElement(TVector2D<int>(64, 96));
		FoorSprites.AddElement(TVector2D<int>(80, 96));
		FoorSprites.AddElement(TVector2D<int>(96, 96));
		FoorSprites.AddElement(TVector2D<int>(112, 96));
		FoorSprites.AddElement(TVector2D<int>(128, 96));

		Sprite.SpriteOffset = FoorSprites.RandomElement();

		SpriteOffset = TVector2D<float>(8);

		ObjectZHeight = -2;

		Sprite.SpriteSize = TVector2D<int>(16);
		break;
	}
	case ETileType::CEILING:
	{
		Sprite.SpriteOffset = TVector2D<int>(160, 16);
		Sprite.SpriteSize = TVector2D<int>(16, 16);

		SpriteOffset = TVector2D<float>(8, 8);

		ObjectZHeight = 2;

		ObjectHitBox.Size = TVector2D<float>(16);
		ObjectHitBox.OffsetFromOrign = TVector2D<float>(8, -8);
		bHasCollision = true;
		break;
	}
	case ETileType::DECOR:
	{
		TArray<TVector2D<int>> Sprites;

		Sprites.AddElement(TVector2D<int>(96, 192));
		Sprites.AddElement(TVector2D<int>(112, 192));
		Sprites.AddElement(TVector2D<int>(128, 192));

		Sprite.SpriteOffset = Sprites.RandomElement();
		Sprite.SpriteSize = TVector2D<int>(16, 16);

		SpriteOffset = TVector2D<float>(8, 8);

		Sprite.RequiredPixelMode = olc::Pixel::MASK;

		if (Sprite.SpriteOffset == TVector2D<int>(96, 192))
		{
			ObjectZHeight = -1;
		}
		else
		{
			ObjectZHeight = 0;
		}

		Sprite.FlipSprite = std::rand() % 2;
		
		break;
	}
	}

}
