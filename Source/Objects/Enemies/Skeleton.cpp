#include "Skeleton.h"
#include "GameCore/GameWorld.h"
#include "Objects/PlayerObject.h"
#include "Objects/Projectiles/AttackProjectile.h"
#include "Objects/Projectiles/BoneProjectile.h"

Skeleton::Skeleton()
{
	WalkSprite.TileSheet = GameWorld::GetWorld()->SkeletonTileSheet;
	WalkSprite.AnimationLength = 0.1f;
	WalkSprite.AnimationType = EAnimationType::LOOP_FOREVER;
	WalkSprite.NumberOfFrames = 10;
	WalkSprite.SpriteOffset = TVector2D<int>(0, 64);
	WalkSprite.SpriteSize = TVector2D<int>(32);
	WalkSprite.RequiredPixelMode = olc::Pixel::MASK;

	IdleSprite.TileSheet = GameWorld::GetWorld()->SkeletonTileSheet;
	IdleSprite.AnimationLength = 0.2f;
	IdleSprite.AnimationType = EAnimationType::LOOP_FOREVER;
	IdleSprite.NumberOfFrames = 10;
	IdleSprite.SpriteOffset = TVector2D<int>(0, 0);
	IdleSprite.SpriteSize = TVector2D<int>(32);
	IdleSprite.RequiredPixelMode = olc::Pixel::MASK;

	AttackSprite.TileSheet = GameWorld::GetWorld()->SkeletonTileSheet;
	AttackSprite.AnimationLength = 0.05f;
	AttackSprite.AnimationType = EAnimationType::LOOP_ONCE;
	AttackSprite.NumberOfFrames = 10;
	AttackSprite.SpriteOffset = TVector2D<int>(0, 96);
	AttackSprite.SpriteSize = TVector2D<int>(32);
	AttackSprite.RequiredPixelMode = olc::Pixel::MASK;

	DeathSprite.TileSheet = GameWorld::GetWorld()->SkeletonTileSheet;
	DeathSprite.AnimationLength = 0.2f;
	DeathSprite.AnimationType = EAnimationType::LOOP_ONCE;
	DeathSprite.NumberOfFrames = 10;
	DeathSprite.SpriteOffset = TVector2D<int>(0, 128);
	DeathSprite.SpriteSize = TVector2D<int>(32);
	DeathSprite.RequiredPixelMode = olc::Pixel::MASK;

	Sprite = IdleSprite;
	bMirrorSpriteOnMovement = true;
	SpriteOffset = TVector2D<float>(8, 16);

	MaxMovementSpeed = 35.f;

	AttackingCoolDown = 4.f;
	bCanAttack = true;

	MaxHealth = 2;
	CurrentHealth = 2;

	ObjectHitBox.bShowHitBoxInGame = false;
	ObjectHitBox.Size = TVector2D<float>(16);
	ObjectHitBox.OffsetFromOrign = TVector2D<float>(0, 4);

	bHasCollision = true;

	DamageImmunities.AddElement(EDamageType::NORMAL);

}

void Skeleton::Tick(float DeltaTime)
{
	if (!bIsAlive)
	{
		return;
	}

	float DistanceToPlayer = (WorldPosition - GameWorld::GetWorld()->Player->WorldPosition).GetAbs().Magnitude();

	if (DistanceToPlayer < 450)
	{
		if (MovementTimer > 5.f)
		{
			RandomMovement();
		}
		MovementTimer += DeltaTime;

		if (LifeTime < DamageTimer && ObjectHitBox.IgnoredObjects.Contains(GameWorld::GetWorld()->Player))
		{
			ObjectHitBox.IgnoredObjects.RemoveElement(GameWorld::GetWorld()->Player);
		}

		if (DistanceToPlayer < 120 && bCanAttack)
		{


			TVector2D<float> Direction = TVector2D<float>(GameWorld::GetWorld()->Player->WorldPosition - WorldPosition).GetNormalised();
			ThrowBone(TVector2D<float>(Direction));

		}
		else
		{
			AddMovementInput(DeltaTime, RandomMovementDirection, 1.f);
		}

	}


	PawnObject::Tick(DeltaTime);
}

void Skeleton::ThrowBone(TVector2D<float> Direction)
{
	BoneProjectile* Projectile = GameWorld::GetWorld()->CreateGameObject<BoneProjectile>(WorldPosition + TVector2D<float>(-8.f, 0.f));
	Projectile->StartProjectileMovement(Direction, this);
	OnAttack();
}

void Skeleton::RandomMovement()
{
	RandomMovementDirection = TVector2D<float>(float((std::rand() % 10) - 5), float((std::rand() % 10) - 5)).GetNormalised();
	MovementTimer = 0.f;
}

void Skeleton::OnCollision(GameObject* OtherObject, bool bWasInstigator)
{
	RandomMovement();

	PlayerObject* Player = dynamic_cast<PlayerObject*>(OtherObject);

	if (Player)
	{
		ObjectHitBox.IgnoredObjects.AddElement(GameWorld::GetWorld()->Player);
		Player->AddImpulseMovement(TVector2D<float>(GameWorld::GetWorld()->Player->WorldPosition - WorldPosition).GetNormalised(), 100.f);
		Player->Damage(1);
		DamageTimer = LifeTime + DamageCoolDown;
	}

	AttackProjectile* Projectile = dynamic_cast<AttackProjectile*>(OtherObject);

	if (Projectile && Projectile->ProjectileOwner != this)
	{
		Damage(1, Projectile->DamageType);
		
		if (!DamageImmunities.Contains(Projectile->DamageType))
		{
			AddImpulseMovement(TVector2D<float>(WorldPosition - Projectile->WorldPosition).GetNormalised(), 200.f);
		}
	}
}
