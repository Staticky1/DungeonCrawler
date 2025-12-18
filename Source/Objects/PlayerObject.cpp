#include "PlayerObject.h"
#include "GameCore/GameWorld.h"
#include "Projectiles/AttackProjectile.h"
#include "Projectiles/MagicProjectile.h"
#include "DoorObject.h"
#include "Objects/Enemies/Snake.h"
#include "Projectiles/BoneProjectile.h"

PlayerObject::PlayerObject()
{
	WalkSprite.TileSheet = GameWorld::GetWorld()->WarriorTileSheet;
	WalkSprite.AnimationLength = 0.1f;
	WalkSprite.AnimationType = EAnimationType::LOOP_FOREVER;
	WalkSprite.NumberOfFrames = 10;
	WalkSprite.SpriteOffset = TVector2D<int>(0, 64);
	WalkSprite.SpriteSize = TVector2D<int>(32);
	WalkSprite.RequiredPixelMode = olc::Pixel::MASK;

	IdleSprite.TileSheet = GameWorld::GetWorld()->WarriorTileSheet;
	IdleSprite.AnimationLength = 0.2f;
	IdleSprite.AnimationType = EAnimationType::LOOP_FOREVER;
	IdleSprite.NumberOfFrames = 10;
	IdleSprite.SpriteOffset = TVector2D<int>(0, 0);
	IdleSprite.SpriteSize = TVector2D<int>(32);
	IdleSprite.RequiredPixelMode = olc::Pixel::MASK;

	AttackSprite.TileSheet = GameWorld::GetWorld()->WarriorTileSheet;
	AttackSprite.AnimationLength = 0.05f;
	AttackSprite.AnimationType = EAnimationType::LOOP_ONCE;
	AttackSprite.NumberOfFrames = 10;
	AttackSprite.SpriteOffset = TVector2D<int>(0, 96);
	AttackSprite.SpriteSize = TVector2D<int>(32);
	AttackSprite.RequiredPixelMode = olc::Pixel::MASK;

	DeathSprite.TileSheet = GameWorld::GetWorld()->WarriorTileSheet;
	DeathSprite.AnimationLength = 0.1f;
	DeathSprite.AnimationType = EAnimationType::LOOP_ONCE;
	DeathSprite.NumberOfFrames = 10;
	DeathSprite.SpriteOffset = TVector2D<int>(0, 128);
	DeathSprite.SpriteSize = TVector2D<int>(32);
	DeathSprite.RequiredPixelMode = olc::Pixel::MASK;

	Sprite = IdleSprite;
	bMirrorSpriteOnMovement = false;
	SpriteOffset = TVector2D<float>(8, 16);

	MaxMovementSpeed = 40.f;

	AttackingCoolDown = 1.f;
	bCanAttack = true;

	ObjectHitBox.bShowHitBoxInGame = false;
	ObjectHitBox.Size = TVector2D<float>(12);
	ObjectHitBox.OffsetFromOrign = TVector2D<float>(-2, 0);

	bHasCollision = true;

	MaxHealth = 6;
	CurrentHealth = 6;
}

PlayerObject::~PlayerObject()
{
}

void PlayerObject::Tick(float DeltaTime)
{
	if (bIsAlive) HandleInput(DeltaTime);

	PawnObject::Tick(DeltaTime);
}

void PlayerObject::HandleInput(float DeltaTime)
{
	float MovementUp = 0.f;
	float MovementLeft = 0.f;

	// Handle User Input
	if (GameWorld::GetWorld()->GetKey(olc::Key::A).bHeld || GameWorld::GetWorld()->GetKey(olc::Key::LEFT).bHeld) MovementLeft += 1.0f;
	if (GameWorld::GetWorld()->GetKey(olc::Key::D).bHeld || GameWorld::GetWorld()->GetKey(olc::Key::RIGHT).bHeld) MovementLeft -= 1.0f;
	if (GameWorld::GetWorld()->GetKey(olc::Key::W).bHeld || GameWorld::GetWorld()->GetKey(olc::Key::UP).bHeld) MovementUp += 1.0f;
	if (GameWorld::GetWorld()->GetKey(olc::Key::S).bHeld || GameWorld::GetWorld()->GetKey(olc::Key::DOWN).bHeld) MovementUp -= 1.0f;

	TVector2D<float> MovementVector(MovementLeft, MovementUp);

	AddMovementInput(DeltaTime, MovementVector, 1.f);

	//If Left mouse spawn normal attack
	if (GameWorld::GetWorld()->GetMouse(0).bHeld && bCanAttack)
	{
		AttackType = EDamageType::NORMAL;
	}
	
	//Right Mouse spawn magic attack
	if (GameWorld::GetWorld()->GetMouse(1).bHeld && bCanAttack)
	{
		AttackType = EDamageType::MAGIC;
	}

	if (GameWorld::GetWorld()->GetMouse(0).bHeld || GameWorld::GetWorld()->GetMouse(1).bHeld)
	{
		if (bCanAttack)
		{
			OnAttack(); 
		}
	}

	CheckSpriteRotation();
}

void PlayerObject::CheckSpriteRotation()
{
	TVector2D<float> MouseScreenSpaceLocation = TVector2D<float>(float(GameWorld::GetWorld()->GetMousePos().x),float(GameWorld::GetWorld()->GetMousePos().y));
	TVector2D<float> MouseWorldSpace = GameWorld::GetWorld()->ScreenPositionToWorldPosition(MouseScreenSpaceLocation);

	TVector2D<float> MouseAngle = (MouseWorldSpace - GameWorld::GetWorld()->Player->WorldPosition).GetNormalised();

	if (MouseAngle.X > 0)
	{
		Sprite.FlipSprite = 1;
	}
	else if (MouseAngle.X < 0)
	{
		Sprite.FlipSprite = 0;
	}
}

void PlayerObject::OnAttack()
{
	PawnObject::OnAttack();

	AttackProjectile* Projectile = nullptr;

	switch (AttackType)
	{
	case EDamageType::NORMAL:
	{
		Projectile = GameWorld::GetWorld()->CreateGameObject<AttackProjectile>(WorldPosition + TVector2D<float>(-8.f, 0.f));
		break;
	}
	case EDamageType::MAGIC:
	{
		Projectile = GameWorld::GetWorld()->CreateGameObject<MagicProjectile>(WorldPosition + TVector2D<float>(-8.f, 0.f));
		break;
	}
	}


	TVector2D<float> MouseScreenSpaceLocation = TVector2D<float>(float(GameWorld::GetWorld()->GetMousePos().x), float(GameWorld::GetWorld()->GetMousePos().y));
	TVector2D<float> MouseWorldSpace = GameWorld::GetWorld()->ScreenPositionToWorldPosition(MouseScreenSpaceLocation);

	TVector2D<float> MouseAngle = (MouseWorldSpace - GameWorld::GetWorld()->Player->WorldPosition).GetNormalised();

	Projectile->StartProjectileMovement(MouseAngle,this);
}

void PlayerObject::OnCollision(GameObject* OtherObject, bool bWasInstigator)
{
	DoorObject* CollisionDoor = dynamic_cast<DoorObject*>(OtherObject);

	if (CollisionDoor)
	{
		CollisionDoor->OpenDoor();
	}

	BoneProjectile* Projectile = dynamic_cast<BoneProjectile*>(OtherObject);

	if (Projectile) //Take damage from bone projectile
	{
		Damage(1);
		AddImpulseMovement(TVector2D<float>(WorldPosition - Projectile->WorldPosition).GetNormalised(), 100.f);
	}
}
