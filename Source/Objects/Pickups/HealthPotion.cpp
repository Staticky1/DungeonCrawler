#include "HealthPotion.h"

#include "GameCore/GameWorld.h"
#include "Objects/PlayerObject.h"
#include "Objects/TextObject.h"

HealthPotion::HealthPotion()
{
	bHasCollision = true;

	ObjectHitBox.bShowHitBoxInGame = false;
	ObjectHitBox.Size = TVector2D<float>(8.f);
	ObjectHitBox.OffsetFromOrign = TVector2D<float>(0.f);

	Sprite.TileSheet = GameWorld::GetWorld()->BackgroundTileSheet;
	Sprite.SpriteOffset = TVector2D<int>(0, 336);
	Sprite.SpriteSize = TVector2D<int>(16);
	Sprite.RequiredPixelMode = olc::Pixel::MASK;
	SpriteOffset = TVector2D<float>(4.f, 8.f);

	bShouldDraw = true;

	ObjectZHeight = 3; //Always draw on top of everything to ensure player can always see

	bTicks = true;
}

void HealthPotion::OnCollision(GameObject* OtherObject, bool bWasInstigator)
{
	PlayerObject* Player = dynamic_cast<PlayerObject*>(OtherObject);

	if (Player)
	{
		if (!bHasCollision) return;

		if (Player->CurrentHealth != Player->MaxHealth)
		{
			//Heal Player

			Player->CurrentHealth = Player->CurrentHealth + 2 > Player->MaxHealth ? Player->MaxHealth : Player->CurrentHealth + 2;

			//DeleteItem
			GameWorld::GetWorld()->DestroyObject(this);

			TextObject* Text = GameWorld::GetWorld()->CreateGameObject<TextObject>(WorldPosition);
			Text->Initalize(GameWorld::GetWorld()->WorldPositionToScreenPosition(WorldPosition) - TVector2D<float>(20.f, 0.f), "HEALED", 2.f);

			bHasCollision = false;
		}

	}
}

void HealthPotion::Tick(float DeltaTime)
{
	//Check if player has collieded
	TArray<GameObject*> Ignore = TArray<GameObject*>({ this });
	GameWorld::GetWorld()->CheckForCollision(ObjectHitBox, WorldPosition, Ignore, this);
}
