#include "AttackProjectile.h"
#include "GameCore/GameWorld.h"

AttackProjectile::AttackProjectile()
{
	Sprite.TileSheet = GameWorld::GetWorld()->AttackEffectsTileSheet;
	Sprite.AnimationLength = 0.06f;
	Sprite.AnimationType = EAnimationType::LOOP_ONCE;
	Sprite.NumberOfFrames = 10;
	Sprite.SpriteOffset = TVector2D<int>(0, 0);
	Sprite.SpriteSize = TVector2D<int>(16);
	Sprite.RequiredPixelMode = olc::Pixel::MASK;
	VerticalSpriteOffset = TVector2D<int>(0, 32);

	LifeSpan = 0.6f;
	MovementSpeed = 80;

	bTicks = true;
	bShouldDraw = true;

	ObjectHitBox.bShowHitBoxInGame = false;
	ObjectHitBox.Shape = EHitBoxShape::SQUARE;
	ObjectHitBox.Size = TVector2D<float>(8.f);
	ObjectHitBox.OffsetFromOrign = TVector2D<float>(4.f, 4.f);

	bHasCollision = true;


	SpriteOffset = TVector2D<float>(8);
	
}

AttackProjectile::~AttackProjectile()
{
}

void AttackProjectile::Tick(float DeltaTime)
{
	WorldPosition += MovementDirection * MovementSpeed * DeltaTime;

	TArray<GameObject*> Ignore = TArray<GameObject*>({ this });
	GameWorld::GetWorld()->CheckForCollision(ObjectHitBox, WorldPosition, Ignore, this);

	if (LifeTime > LifeSpan)
	{
		GameWorld::GetWorld()->DestroyObject(this);
	}
}

void AttackProjectile::BeginPlay()
{
}

void AttackProjectile::StartProjectileMovement(TVector2D<float> Direction, GameObject* Owner)
{
	MovementDirection = Direction;
	ProjectileOwner = Owner;

	ObjectHitBox.IgnoredObjects.AddElement(Owner);

	//Should we flip vertically
	if (FMath::Abs(Direction.X) < FMath::Abs(Direction.Y) && bAllowVertFlip)
	{
		Sprite.SpriteOffset = VerticalSpriteOffset;
		
		if (Direction.Y > 0)
		{
			Sprite.FlipSprite = 2;
		}
		else if (Direction.Y < 0)
		{
			Sprite.FlipSprite = 0;
		}
	}
	else
	{
		if (Direction.X > 0)
		{
			Sprite.FlipSprite = 1;
		}
		else if (Direction.X < 0)
		{
			Sprite.FlipSprite = 0;
		}
	}

}

void AttackProjectile::OnCollision(GameObject* OtherObject, bool bWasInstigator)
{
	if (OtherObject != ProjectileOwner)
	{
		GameWorld::GetWorld()->DestroyObject(this);
	}
}
