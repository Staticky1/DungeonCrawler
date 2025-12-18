#include "Snake.h"
#include "GameCore/GameWorld.h"
#include "Objects/PlayerObject.h"
#include "Objects/Projectiles/AttackProjectile.h"

Snake::Snake()
{
	
	WalkSprite.TileSheet = GameWorld::GetWorld()->SnakeTileSheet;
	WalkSprite.AnimationLength = 0.1f;
	WalkSprite.AnimationType = EAnimationType::LOOP_FOREVER;
	WalkSprite.NumberOfFrames = 10;
	WalkSprite.SpriteOffset = TVector2D<int>(0, 64);
	WalkSprite.SpriteSize = TVector2D<int>(32);
	WalkSprite.RequiredPixelMode = olc::Pixel::MASK;

	IdleSprite.TileSheet = GameWorld::GetWorld()->SnakeTileSheet;
	IdleSprite.AnimationLength = 0.2f;
	IdleSprite.AnimationType = EAnimationType::LOOP_FOREVER;
	IdleSprite.NumberOfFrames = 10;
	IdleSprite.SpriteOffset = TVector2D<int>(0, 0);
	IdleSprite.SpriteSize = TVector2D<int>(32);
	IdleSprite.RequiredPixelMode = olc::Pixel::MASK;

	AttackSprite.TileSheet = GameWorld::GetWorld()->SnakeTileSheet;
	AttackSprite.AnimationLength = 0.05f;
	AttackSprite.AnimationType = EAnimationType::LOOP_ONCE;
	AttackSprite.NumberOfFrames = 10;
	AttackSprite.SpriteOffset = TVector2D<int>(0, 96);
	AttackSprite.SpriteSize = TVector2D<int>(32);
	AttackSprite.RequiredPixelMode = olc::Pixel::MASK;

	DeathSprite.TileSheet = GameWorld::GetWorld()->SnakeTileSheet;
	DeathSprite.AnimationLength = 0.2f;
	DeathSprite.AnimationType = EAnimationType::LOOP_ONCE;
	DeathSprite.NumberOfFrames = 10;
	DeathSprite.SpriteOffset = TVector2D<int>(0, 128);
	DeathSprite.SpriteSize = TVector2D<int>(32);
	DeathSprite.RequiredPixelMode = olc::Pixel::MASK;

	Sprite = IdleSprite;
	bMirrorSpriteOnMovement = true;
	SpriteOffset = TVector2D<float>(8, 16);

	MaxMovementSpeed = 25.f;
	LungeSpeed = 200.f;
	Friction = 450.f;

	AttackingCoolDown = 4.f;
	bCanAttack = true;

	ObjectHitBox.bShowHitBoxInGame = false;
	ObjectHitBox.Size = TVector2D<float>(8);
	ObjectHitBox.OffsetFromOrign = TVector2D<float>(-4,-4);

	bHasCollision = true;
}

void Snake::Tick(float DeltaTime)
{
	if (!bIsAlive)
	{
		return;
	}
	
	float DistanceToPlayer = (WorldPosition - GameWorld::GetWorld()->Player->WorldPosition).GetAbs().Magnitude();

	if (DistanceToPlayer < 450) //Only do stuff if the player is close
	{
		if (MovementTimer > 5.f)
		{
			RandomMovement();
		}
		MovementTimer += DeltaTime;

		if (LifeTime < AtkDamageTimer && ObjectHitBox.IgnoredObjects.Contains(GameWorld::GetWorld()->Player))
		{
			ObjectHitBox.IgnoredObjects.RemoveElement(GameWorld::GetWorld()->Player);
		}

		if (DistanceToPlayer < LungeRange && bCanAttack)
		{
			
			
			TVector2D<float> Direction = TVector2D<float>(GameWorld::GetWorld()->Player->WorldPosition - WorldPosition).GetNormalised();
			LungeAttack(TVector2D<float>(Direction));
			
		}
		else if (!bIsAttacking)
		{
			AddMovementInput(DeltaTime, RandomMovementDirection, 1.f);
		}

		PawnObject::Tick(DeltaTime);
	}
	
}

void Snake::LungeAttack(TVector2D<float> Direction)
{
	AddImpulseMovement(Direction, LungeSpeed);
	OnAttack();
}

void Snake::RandomMovement()
{
	RandomMovementDirection = TVector2D<float>(float((std::rand() % 10) - 5), float((std::rand() % 10) - 5)).GetNormalised();
	MovementTimer = 0.f;
}

void Snake::OnCollision(GameObject* OtherObject, bool bWasInstigator)
{
	RandomMovement();

	PlayerObject* Player = dynamic_cast<PlayerObject*>(OtherObject);

	if (Player)
	{
		ObjectHitBox.IgnoredObjects.AddElement(GameWorld::GetWorld()->Player);
		Player->AddImpulseMovement(TVector2D<float>(GameWorld::GetWorld()->Player->WorldPosition - WorldPosition).GetNormalised(), KnockbackForce);
		if (AtkDamageTimer < LifeTime)
		{
			Player->Damage(1);
		}
		AtkDamageTimer = LifeTime + AtkDamageCoolDown;
		
	}

	AttackProjectile* Projectile = dynamic_cast<AttackProjectile*>(OtherObject);

	if (Projectile && Projectile->ProjectileOwner == GameWorld::GetWorld()->Player)
	{
		if (!bIsAttacking)
		{
			AddImpulseMovement(TVector2D<float>(WorldPosition - Projectile->WorldPosition).GetNormalised(), 50.f);
		}
		Damage(1, Projectile->DamageType);
		//DamageTimer = LifeTime + DamageCoolDown;
		
	}
}
