#include "Minotaur.h"
#include "GameCore/GameWorld.h"

Minotaur::Minotaur()
{
	WalkSprite.TileSheet = GameWorld::GetWorld()->MinotaurTileSheet;
	WalkSprite.AnimationLength = 0.1f;
	WalkSprite.AnimationType = EAnimationType::LOOP_FOREVER;
	WalkSprite.NumberOfFrames = 10;
	WalkSprite.SpriteOffset = TVector2D<int>(0, 96);
	WalkSprite.SpriteSize = TVector2D<int>(48);
	WalkSprite.RequiredPixelMode = olc::Pixel::MASK;

	IdleSprite.TileSheet = GameWorld::GetWorld()->MinotaurTileSheet;
	IdleSprite.AnimationLength = 0.2f;
	IdleSprite.AnimationType = EAnimationType::LOOP_FOREVER;
	IdleSprite.NumberOfFrames = 10;
	IdleSprite.SpriteOffset = TVector2D<int>(0, 0);
	IdleSprite.SpriteSize = TVector2D<int>(48);
	IdleSprite.RequiredPixelMode = olc::Pixel::MASK;

	AttackSprite.TileSheet = GameWorld::GetWorld()->MinotaurTileSheet;
	AttackSprite.AnimationLength = 0.05f;
	AttackSprite.AnimationType = EAnimationType::LOOP_ONCE;
	AttackSprite.NumberOfFrames = 10;
	AttackSprite.SpriteOffset = TVector2D<int>(0, 144);
	AttackSprite.SpriteSize = TVector2D<int>(48);
	AttackSprite.RequiredPixelMode = olc::Pixel::MASK;

	DeathSprite.TileSheet = GameWorld::GetWorld()->MinotaurTileSheet;
	DeathSprite.AnimationLength = 0.2f;
	DeathSprite.AnimationType = EAnimationType::LOOP_ONCE;
	DeathSprite.NumberOfFrames = 10;
	DeathSprite.SpriteOffset = TVector2D<int>(0, 192);
	DeathSprite.SpriteSize = TVector2D<int>(48);
	DeathSprite.RequiredPixelMode = olc::Pixel::MASK;

	Sprite = IdleSprite;
	bMirrorSpriteOnMovement = true;
	SpriteOffset = TVector2D<float>(8, 16);

	MaxMovementSpeed = 45.f;
	LungeSpeed = 250.f;
	LungeRange = 180.f;
	KnockbackForce = 250.f;
	Friction = 250.f;

	AttackingCoolDown = 3.f;
	bCanAttack = true;

	MaxHealth = 5;
	CurrentHealth = 5;

	ObjectHitBox.bShowHitBoxInGame = false;
	ObjectHitBox.Size = TVector2D<float>(32);
	ObjectHitBox.OffsetFromOrign = TVector2D<float>(0, 0);

	bHasCollision = true;
}
