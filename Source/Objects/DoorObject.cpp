#include "DoorObject.h"
#include "GameCore/GameWorld.h"
#include "BackgroundTile.h"
#include "PlayerObject.h"
#include "TextObject.h"

DoorObject::DoorObject()
{


	ObjectHitBox.bShowHitBoxInGame = false;

	OpenSprite.TileSheet = GameWorld::GetWorld()->BackgroundTileSheet;
	ClosedSprite.TileSheet = GameWorld::GetWorld()->BackgroundTileSheet;

	bShouldDraw = true;
	bTicks = true;
}

void DoorObject::CreateDoor(int DoorDirection, bool bStartLocked)
{
	DoorDirectionVar = DoorDirection;

	switch (DoorDirection)
	{
	case 0: // North/South Facing
	{
		OpenSprite.SpriteOffset = TVector2D<int>(112, 128);
		OpenSprite.SpriteSize = TVector2D<int>(32, 48);
		OpenSprite.RequiredPixelMode = olc::Pixel::MASK;
		OpenSpriteOffset = TVector2D<float>(16, 16);

		ClosedSprite.SpriteOffset = TVector2D<int>(0, 128);
		ClosedSprite.SpriteSize = TVector2D<int>(32, 32);
		ClosedSpriteOffset = TVector2D<float>(16, 16);
		ClosedSprite.RequiredPixelMode = olc::Pixel::MASK;
		

		ObjectHitBox.Size = TVector2D<float>(32, 16);
		ObjectHitBox.OffsetFromOrign = TVector2D<float>(16, 0);
		break;

	}
	case 1: //West facing, East wall
	{
		OpenSprite.SpriteOffset = TVector2D<int>(48, 128);
		OpenSprite.SpriteSize = TVector2D<int>(16, 32);
		OpenSprite.RequiredPixelMode = olc::Pixel::MASK;
		OpenSpriteOffset = TVector2D<float>(16, 0);
		OpenSprite.FlipSprite = 0;

		ClosedSprite.SpriteOffset = TVector2D<int>(144, 192);
		ClosedSprite.SpriteSize = TVector2D<int>(16, 64);
		ClosedSpriteOffset = TVector2D<float>(15, 32);
		ClosedSprite.FlipSprite = 1;
		ClosedSprite.RequiredPixelMode = olc::Pixel::MASK;

		ObjectHitBox.Size = TVector2D<float>(16, 48);
		ObjectHitBox.OffsetFromOrign = TVector2D<float>(0,16);
		break;
	}
	case 2: //East Facing, west wall
	{
		OpenSprite.SpriteOffset = TVector2D<int>(48, 128);
		OpenSprite.SpriteSize = TVector2D<int>(16, 48);
		OpenSpriteOffset = TVector2D<float>(-16, 0);
		OpenSprite.RequiredPixelMode = olc::Pixel::MASK;

		ClosedSprite.SpriteOffset = TVector2D<int>(144, 192);
		ClosedSprite.SpriteSize = TVector2D<int>(16, 64);
		ClosedSpriteOffset = TVector2D<float>(-15, 32);
		ClosedSprite.RequiredPixelMode = olc::Pixel::MASK;
		ClosedSprite.FlipSprite = 0;

		ObjectHitBox.Size = TVector2D<float>(16, 48);
		ObjectHitBox.OffsetFromOrign = TVector2D<float>(0, 16);
		break;
	}
	}

	bIsLocked = bStartLocked;

	Sprite = ClosedSprite;
	SpriteOffset = ClosedSpriteOffset;

	bHasCollision = true;
}

void DoorObject::OpenDoor()
{
	if (!bIsLocked)
	{
		Sprite = OpenSprite;
		SpriteOffset = OpenSpriteOffset;

		if (DoorDirectionVar == 1 || DoorDirectionVar == 2)
		{
			BackgroundTile* TopDoorObject = GameWorld::GetWorld()->CreateGameObject<BackgroundTile>(WorldPosition + TVector2D<float>(0, -16));


			TopDoorObject->Sprite.SpriteOffset = TVector2D<int>(48, 128);
			TopDoorObject->Sprite.SpriteSize = TVector2D<int>(16, 32);

			TopDoorObject->SpriteOffset = TVector2D<float>(16, 48);

			if (DoorDirectionVar == 1)
			{
				TopDoorObject->Sprite.FlipSprite = 0;
			}
			else
			{
				TopDoorObject->SpriteOffset = TVector2D<float>(-16, 48);
			}
		}

		bHasCollision = false;

		return;
	}
	else
	{
		if (GameWorld::GetWorld()->Player->bIsHoldingKey)
		{
			bIsLocked = false;

			GameWorld::GetWorld()->Player->bIsHoldingKey = false;

			if (bCanDisplayText)
			{
				TextObject* Text = GameWorld::GetWorld()->CreateGameObject<TextObject>(WorldPosition);
				Text->Initalize(GameWorld::GetWorld()->WorldPositionToScreenPosition(WorldPosition) - TVector2D<float>(20.f, 0.f), "UNLOCKED", 2.f);
				bCanDisplayText = false;
				DisplayTextTimer = LifeTime + DisplayTextCoolDown;
			}

			OpenDoor();
		}
		else
		{
			if (bCanDisplayText)
			{
				TextObject* Text = GameWorld::GetWorld()->CreateGameObject<TextObject>(WorldPosition);
				Text->Initalize(GameWorld::GetWorld()->WorldPositionToScreenPosition(WorldPosition) - TVector2D<float>(20.f, 0.f), "LOCKED", 2.f);
				bCanDisplayText = false;
				DisplayTextTimer = LifeTime + DisplayTextCoolDown;
			}
		}
	}
	

}

void DoorObject::Tick(float DeltaTime)
{
	if (LifeTime > DisplayTextTimer && !bCanDisplayText)
	{
		bCanDisplayText = true;
	}
}
