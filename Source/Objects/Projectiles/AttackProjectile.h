#pragma once

#include "GameCore/GameObject.h"

//Base class for all projectiles
// contains logic for projectile movement and lifespan

enum class EDamageType
{
	NORMAL,
	MAGIC,
};


class AttackProjectile : public GameObject
{

public:
	AttackProjectile();
	~AttackProjectile();

	virtual void Tick(float DeltaTime) override;

	void BeginPlay() override;

	//how long the projectile exsits before destruction
	float LifeSpan = 1.f;

	float MovementSpeed = 50.f;
	TVector2D<float> MovementDirection;

	void StartProjectileMovement(TVector2D<float> Direction, GameObject* Owner);

	//ProjectileOwner is ignored by collision checking
	GameObject* ProjectileOwner = nullptr;

	//Allow sprite to flip based on direction
	bool bAllowVertFlip = true;
	//offset for vertical sprite
	TVector2D<int> VerticalSpriteOffset;

	virtual void OnCollision(GameObject* OtherObject, bool bWasInstigator) override;

	//The Damage type that this projectile deals
	EDamageType DamageType = EDamageType::NORMAL;




};