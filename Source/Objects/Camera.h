#pragma once

#include "GameCore/GameObject.h"

// Camera object control where the viewport is, this class attempts to stay in between the mouse and the player

class Camera : public GameObject
{
public:
	Camera();
	~Camera();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:

	float LerpSpeed = 5.f;
	float MaxDistance = 20.f;
};