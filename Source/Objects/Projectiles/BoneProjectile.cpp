#include "BoneProjectile.h"
#include "GameCore/GameWorld.h"

BoneProjectile::BoneProjectile()
{
	Sprite.TileSheet = GameWorld::GetWorld()->BoneTileSheet;
	Sprite.AnimationLength = 0.05f;
	Sprite.AnimationType = EAnimationType::LOOP_FOREVER;
	Sprite.NumberOfFrames = 10;
	Sprite.SpriteOffset = TVector2D<int>(0, 0);
	Sprite.SpriteSize = TVector2D<int>(16);
	Sprite.RequiredPixelMode = olc::Pixel::MASK;

	LifeSpan = 1.f;
	MovementSpeed = 100;

	bTicks = true;
	bShouldDraw = true;

	ObjectHitBox.bShowHitBoxInGame = false;
	ObjectHitBox.Shape = EHitBoxShape::SQUARE;
	ObjectHitBox.Size = TVector2D<float>(8.f);
	ObjectHitBox.OffsetFromOrign = TVector2D<float>(4.f,4.f);

	bHasCollision = true;

	bAllowVertFlip = false;
	SpriteOffset = TVector2D<float>(8);
}
