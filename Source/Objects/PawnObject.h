#pragma once

#include "GameCore/GameObject.h"
#include "Projectiles/AttackProjectile.h"


enum class EAnimationState
{
	IDLE,
	GESTURE,
	WALK,
	ATTACK,
	DEATH,
};


//
//  Pawn object contains logic for swapping animations, and movement  
//


class PawnObject : public GameObject
{
public:
	PawnObject();
	~PawnObject();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	//The current animation state determines what animations are currently playing on the sprite
	EAnimationState CurrentAnimationState = EAnimationState::IDLE;

	//Animations
	//calculates what the current animation state should be
	virtual void CalculateAnimationState();

	virtual void UpdateAnimationState(EAnimationState NewState);

	//should the sprite face the direction of movement
	bool bMirrorSpriteOnMovement = true;

	FSprite IdleSprite;
	FSprite WalkSprite;
	FSprite AttackSprite;
	FSprite DeathSprite;

	virtual void OnAnimationFinish() override;

	//Death
	bool bIsAlive = true;

	virtual void OnDeath();

	//Called when the death animation finishes
	virtual void CreateLoot();

	bool bIsHoldingKey = false;
	bool bHasHealthPotion = false;

	//Health

	int MaxHealth = 1;
	int CurrentHealth = 1;

	void Heal(int Amount);
	void Damage(int Amount, EDamageType Type = EDamageType::NORMAL);

	float DamageCoolDown = 0.5f;
	float DamageTime = 0.f;
	bool bCanBeDamaged = true;

	//Movement speed
	float MaxMovementSpeed = 25.f;

	TVector2D<float> InputMovementForce;
	TVector2D<float> CurrentMovementDirection;
	TVector2D<float> ImpluseMovementForce;

	float Friction = 300.f;
	float ImpluseStrenght = 0.f;

	//Sets the inputmovement direction
	virtual void AddMovementInput(float DeltaTime, TVector2D<float> Direction, float InputValue);

	//Adds a impulse that is added to the current movement direction
	virtual void AddImpulseMovement(TVector2D<float> ImpulseDirection, float InImpluseStrength);

	//Computes all movement and moves pawn object
	virtual void HandleMovement(float DeltaTime);

	//Attacking

	bool bIsAttacking = false;
	bool bCanAttack = true;
	float AttackingCoolDown = 1.f;
	float AttackTime = 0.f;

	//Function Called when an attack is made
	virtual void OnAttack();

	TArray<EDamageType> DamageImmunities;

private:

	//Is Set to true
	bool bIsInitilized = false;

	//checked before changing an animation state, used to make sure an entire animation is played in full
	bool bCanInteruptAnimation = true;
};