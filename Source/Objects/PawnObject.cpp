#include "PawnObject.h"
#include "GameCore/GameWorld.h"
#include "Pickups/KeyPickup.h"
#include "Pickups/HealthPotion.h"
#include "TextObject.h"

PawnObject::PawnObject()
{
	bTicks = true;
	bShouldDraw = true;

}

PawnObject::~PawnObject()
{
}

void PawnObject::Tick(float DeltaTime)
{
	if (!bIsAlive) return;
	
	if (bIsInitilized)
	{
		CalculateAnimationState();
	}

	if (!bCanAttack && LifeTime > AttackTime + AttackingCoolDown)
	{
		bCanAttack = true;
	}

	if (!bCanBeDamaged && LifeTime > DamageTime + DamageCoolDown)
	{
		bCanBeDamaged = true;
	}

	HandleMovement(DeltaTime);
}

void PawnObject::BeginPlay()
{
	bIsInitilized = true;
}

void PawnObject::CalculateAnimationState()
{
	if (bMirrorSpriteOnMovement)
	{
		//Mirror the sprite based on direction moving
		if (CurrentVelocity.X > 0)
		{
			Sprite.FlipSprite = 0;
		}
		else if (CurrentVelocity.X < 0)
		{
			Sprite.FlipSprite = 1;
		}
	}

	//switch between walking an idle
	if (FMath::Abs(CurrentVelocity.Magnitude()) > 0 && CurrentAnimationState != EAnimationState::WALK && bCanInteruptAnimation)
	{
		UpdateAnimationState(EAnimationState::WALK);
	}
	else if (FMath::Abs(CurrentVelocity.Magnitude()) == 0 && CurrentAnimationState != EAnimationState::IDLE && bCanInteruptAnimation)
	{
		UpdateAnimationState(EAnimationState::IDLE);
	}

	if (bIsAttacking && CurrentAnimationState != EAnimationState::ATTACK)
	{
		UpdateAnimationState(EAnimationState::ATTACK);
		bCanInteruptAnimation = false;
	}

}

void PawnObject::UpdateAnimationState(EAnimationState NewState)
{
	CurrentAnimationState = NewState;
	bool PrevFlip = Sprite.FlipSprite;

	switch (NewState)
	{
	case EAnimationState::IDLE:
		Sprite = IdleSprite;
		break;
	case EAnimationState::WALK:
		Sprite = WalkSprite;
		break;
	case EAnimationState::ATTACK:
		Sprite = AttackSprite;
		break;
	case EAnimationState::DEATH:
		Sprite = DeathSprite;
		break;
	}

	//presruve the flip value 
	Sprite.FlipSprite = PrevFlip;
}

void PawnObject::AddMovementInput(float DeltaTime, TVector2D<float> Direction, float InputValue)
{
	InputMovementForce = Direction * InputValue * MaxMovementSpeed * DeltaTime;
}

void PawnObject::AddImpulseMovement(TVector2D<float> ImpulseDirection, float InImpluseStrength)
{
	if (ImpulseDirection.IsNormalised())
	{
		ImpluseMovementForce = ImpulseDirection;
		ImpluseStrenght = InImpluseStrength;
	}
}

void PawnObject::HandleMovement(float DeltaTime)
{
	ImpluseStrenght = ImpluseStrenght - Friction * DeltaTime > 0.f ? ImpluseStrenght - Friction * DeltaTime : 0.f;

	TVector2D<float> CurrentImpluse = ImpluseMovementForce * ImpluseStrenght * DeltaTime;
		

	TVector2D<float> NewMovement = (CurrentImpluse) + InputMovementForce;

	// Check for collisions before moving to stop moving into walls etc
	TArray<GameObject*> ObjectsToIgnore = TArray<GameObject*>({ this });
	if (bHasCollision)
	{
		bool bHit = GameWorld::GetWorld()->CheckForCollision(ObjectHitBox, NewMovement + WorldPosition, ObjectsToIgnore,this, true);
		
		if (bHit)
		{
			ImpluseStrenght = 0.f;
			WorldPosition -= InputMovementForce;
			return;
		}

	}

	

	WorldPosition += NewMovement;
}

void PawnObject::OnAttack()
{
	bIsAttacking = true;
	AttackTime = LifeTime;
	bCanAttack = false;


}

void PawnObject::OnAnimationFinish()
{
	if (!bCanInteruptAnimation)
	{
		bCanInteruptAnimation = true;
	}

	

	if (bIsAttacking)
	{
		bIsAttacking = false;
	}

	if (!bIsAlive)
	{
		
	}
}

void PawnObject::OnDeath()
{
	bIsAlive = false;
	UpdateAnimationState(EAnimationState::DEATH);
	bTicks = false;
	bHasCollision = false;

	CreateLoot();
}

void PawnObject::CreateLoot()
{
	//Create a Key
	if (bIsHoldingKey)
	{
		GameWorld::GetWorld()->CreateGameObject<KeyPickup>(WorldPosition );
	}

	if (bHasHealthPotion)
	{
		GameWorld::GetWorld()->CreateGameObject<HealthPotion>(WorldPosition);
	}
}

void PawnObject::Heal(int Amount)
{
	CurrentHealth = CurrentHealth + Amount > MaxHealth ? MaxHealth : CurrentHealth + Amount;
}

void PawnObject::Damage(int Amount, EDamageType Type)
{
	
	if (!bCanBeDamaged) return;

	//If Immune to damage type, ignore damage
	if (DamageImmunities.Contains(Type))
	{
		TextObject* Text = GameWorld::GetWorld()->CreateGameObject<TextObject>(WorldPosition);
		Text->Initalize(GameWorld::GetWorld()->WorldPositionToScreenPosition(WorldPosition) - TVector2D<float>(20.f, 0.f), "RESIST", 1.f);
		return;
	}

	CurrentHealth = CurrentHealth - Amount <= 0 ? 0 : CurrentHealth - Amount;

	//Invincabiltiy Frames
	bCanBeDamaged = false;
	DamageTime = LifeTime + DamageCoolDown;

	if (CurrentHealth == 0)
	{
		OnDeath();
	}
}
