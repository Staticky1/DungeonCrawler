#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::BeginPlay()
{
}

void GameObject::Tick(float DeltaTime)
{

}

void GameObject::PostTick(float DeltaTime)
{
    LifeTime += DeltaTime;

    SetCurrentVelocity();

    WorldPositionLastFrame = WorldPosition;
    PreviousDeltaTime = DeltaTime;
}

ETickGroup GameObject::GetTickGroup() const
{
    return TickGroup;
}

void GameObject::OnDestruction()
{
}

void GameObject::OnAnimationFinish()
{
}

void GameObject::OnCollision(GameObject* OtherObject, bool bWasInstigator)
{
}

void GameObject::SetCurrentVelocity()
{
    CurrentVelocity = (WorldPosition - WorldPositionLastFrame) * PreviousDeltaTime;
}

