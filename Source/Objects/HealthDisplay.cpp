#include "HealthDisplay.h"

#include "PlayerObject.h"
#include "GameCore/GameWorld.h"

HealthDisplay::HealthDisplay()
{
	bTicks = true;
	TickGroup = ETickGroup::POSTWORK;

	FullHeartOffset = TVector2D<int>(0);
	HalfHeartOffset = TVector2D<int>(19, 0);
	EmptyHeartOffset = TVector2D<int>(38, 0);

	SpriteSheet = GameWorld::GetWorld()->HeartTileSheet;
}

void HealthDisplay::Tick(float DeltaTime)
{
	if (Player)
	{
		UpdateDisplay();
	}
}

void HealthDisplay::InitalizeDisplay(PlayerObject* InPlayer, TVector2D<int> ScreenPos)
{
	Player = InPlayer;
	ScreenLocation = ScreenPos;
}

void HealthDisplay::UpdateDisplay()
{
	for (int HeartIndex = 1; HeartIndex < (Player->MaxHealth / 2)+ 1; HeartIndex++)
	{
		GameWorld::GetWorld()->SetPixelMode(olc::Pixel::MASK);

		if (Player->CurrentHealth >= HeartIndex * 2)
		{
			//full heart
			GameWorld::GetWorld()->DrawPartialSprite(olc::vi2d(ScreenLocation.X + HeartIndex * 20, ScreenLocation.Y), SpriteSheet->Get(), olc::vi2d(FullHeartOffset.X, FullHeartOffset.Y), olc::vi2d(18, 15));
		}
		if (Player->CurrentHealth < HeartIndex * 2)
		{
			if (Player->CurrentHealth == HeartIndex * 2 - 1)
			{
				//half heart
				GameWorld::GetWorld()->DrawPartialSprite(olc::vi2d(ScreenLocation.X + HeartIndex * 20, ScreenLocation.Y), SpriteSheet->Get(), olc::vi2d(HalfHeartOffset.X, HalfHeartOffset.Y), olc::vi2d(18, 15));
			}
			else
			{
				//empty heart
				GameWorld::GetWorld()->DrawPartialSprite(olc::vi2d(ScreenLocation.X + HeartIndex * 20, ScreenLocation.Y), SpriteSheet->Get(), olc::vi2d(EmptyHeartOffset.X, EmptyHeartOffset.Y), olc::vi2d(18, 15));
			}
		}
	}
}
