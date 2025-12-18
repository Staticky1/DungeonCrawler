#include "WinTrigger.h"
#include "PlayerObject.h"
#include "GameCore/GameWorld.h"
#include "TextObject.h"


WinTrigger::WinTrigger()
{
	ObjectHitBox.bShowHitBoxInGame = false;
	ObjectHitBox.Size = TVector2D<float>(32);
	ObjectHitBox.OffsetFromOrign = TVector2D<float>(-12,8);
	Sprite.TileSheet = GameWorld::GetWorld()->BackgroundTileSheet;

	bShouldDraw = false;
	bHasCollision = true;
}

void WinTrigger::OnCollision(GameObject* OtherObject, bool bWasInstigator)
{
	PlayerObject* Player = dynamic_cast<PlayerObject*>(OtherObject);

	if (Player)
	{
		TextObject* Text = GameWorld::GetWorld()->CreateGameObject<TextObject>();
		Text->Initalize(TVector2D<float>(70, 100), "YOU WIN", 20.f, 2);

		bHasCollision = false;
	}
}
