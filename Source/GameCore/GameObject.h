#pragma once

#include "Core.h"


//Game Object Class
//
// The base class for all spawnable objects in a world
//

class ObjectComponent;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	//Called on the first tick after an object is created
	virtual void BeginPlay();

	//Called on each game tick, as long as bTicks is set to true
	virtual void Tick(float DeltaTime);

	//Called after the tick function, used for calculating changes etc
	void PostTick(float DeltaTime);

	//Returns the tickgroup of this object
	ETickGroup GetTickGroup() const;

	//Called on objects destruction before destructor
	virtual void OnDestruction();

	//Position of the object in world space
	TVector2D<float> WorldPosition = TVector2D<float>(0.f,0.f);

	//The sprite that this object shows
	FSprite Sprite;

	//Offset where the sprite is shown in relation to the worldpos of the object
	TVector2D<float> SpriteOffset;

	//A modifier that changes the height at which an object is drawn. Objects with a lower Z height will always be drawn under objects with a higher Z height
	int ObjectZHeight = 0;

	//Does the object tick;
	bool bTicks = false;

	//Should this objects sprite be drawn in game
	bool bShouldDraw = false;

	//Total time this object has been in the world
	float LifeTime = 0.f;

	//Called when the last frame of an animation is finished played
	virtual void OnAnimationFinish();

	//The current movement speed of the object
	TVector2D<float> CurrentVelocity = TVector2D<float>(0.f, 0.f);

	//Objects hit box
	FHitBox ObjectHitBox;

	//should the object check for collision every frame
	bool bConstantCollisionChecking = false;

	//Should the object be included in collision calculations
	bool bHasCollision = false;

	virtual void OnCollision(GameObject* OtherObject, bool bWasInstigator);

	bool IsReadyForDestruction = false;

private:
	//caluclates the current speed of the object
	void SetCurrentVelocity();

	//The world position on the previous frame, used to calculate velocity
	TVector2D<float> WorldPositionLastFrame = TVector2D<float>(0.f, 0.f);

	//Used for calculating velocity, etc
	float PreviousDeltaTime = 0.f;

protected:

	ETickGroup TickGroup = ETickGroup::WORKGROUP;




};