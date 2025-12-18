#include "MagicProjectile.h"
#include <GameCore/GameWorld.h>

MagicProjectile::MagicProjectile()
{
	Sprite.TileSheet = GameWorld::GetWorld()->AttackEffectsTileSheet;
	Sprite.AnimationLength = 0.1f;
	Sprite.AnimationType = EAnimationType::LOOP_ONCE;
	Sprite.NumberOfFrames = 10;
	Sprite.SpriteOffset = TVector2D<int>(0, 16);
	Sprite.SpriteSize = TVector2D<int>(16);
	Sprite.RequiredPixelMode = olc::Pixel::MASK;
	VerticalSpriteOffset = TVector2D<int>(0, 48);

	LifeSpan = 1.0f;
	MovementSpeed = 40;

	bTicks = true;
	bShouldDraw = true;

	ObjectHitBox.bShowHitBoxInGame = false;
	ObjectHitBox.Shape = EHitBoxShape::SQUARE;
	ObjectHitBox.Size = TVector2D<float>(8.f);
	ObjectHitBox.OffsetFromOrign = TVector2D<float>(4.f, 4.f);

	bHasCollision = true;

	DamageType = EDamageType::MAGIC; //Magic damage type

	SpriteOffset = TVector2D<float>(8);
}
