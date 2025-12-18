#include "TextObject.h"
#include "GameCore/GameWorld.h"

TextObject::TextObject()
{
	bTicks = true;
	TickGroup = ETickGroup::POSTWORK;
}

void TextObject::CreateText(TVector2D<float> ScreenLocation, std::string Text, int FontSize)
{
	int TotalXLength = int(ScreenLocation.X);

	GameWorld::GetWorld()->SetPixelMode(olc::Pixel::MASK);

	for (auto& Character : Text)
	{
		int CharacterIndex = -1;

		for (int Index = 0; Index < FontArray.Num(); Index++)
		{
			if (FontArray[Index].Character == Character)
			{
				CharacterIndex = Index;
				break;
			}
		}

		if (CharacterIndex == -1)
		{
			break;
		}
		else
		{


			olc::vi2d ScreenPos = olc::vi2d(TotalXLength + 0, int(ScreenLocation.Y));
			olc::vi2d SpriteOffset = olc::vi2d(FontArray[CharacterIndex].Location.X, FontArray[CharacterIndex].Location.Y);
			olc::vi2d SpriteSize = olc::vi2d(FontArray[CharacterIndex].Size.X, FontArray[CharacterIndex].Size.Y);

			

			GameWorld::GetWorld()->DrawPartialSprite(ScreenPos, FontSprite->Get(), SpriteOffset, SpriteSize, FontSize);

			TotalXLength += (FontArray[CharacterIndex].Size.X) * FontSize;
		}
	}
}

void TextObject::Initalize(TVector2D<float> ScreenLocation, std::string Text, float TimeOnScreen, int FontSize)
{
	FontSprite = GameWorld::GetWorld()->FontTileSheet;

	FontArray.AddElement(FText('A', TVector2D<int>(1, 27), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('B', TVector2D<int>(9, 27), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('C', TVector2D<int>(17, 27), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('D', TVector2D<int>(25, 27), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('E', TVector2D<int>(33, 27), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('F', TVector2D<int>(41, 27), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('G', TVector2D<int>(49, 27), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('H', TVector2D<int>(57, 27), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('I', TVector2D<int>(65, 27), TVector2D<int>(4, 8)));
	FontArray.AddElement(FText('J', TVector2D<int>(70, 27), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('K', TVector2D<int>(78, 27), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('L', TVector2D<int>(86, 27), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('M', TVector2D<int>(94, 27), TVector2D<int>(9, 8)));
	FontArray.AddElement(FText('N', TVector2D<int>(1, 36), TVector2D<int>(8, 8)));
	FontArray.AddElement(FText('O', TVector2D<int>(10, 36), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('P', TVector2D<int>(18, 36), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('Q', TVector2D<int>(26, 36), TVector2D<int>(8, 8)));
	FontArray.AddElement(FText('R', TVector2D<int>(35, 36), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('S', TVector2D<int>(43, 36), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('T', TVector2D<int>(51, 36), TVector2D<int>(8, 8)));
	FontArray.AddElement(FText('U', TVector2D<int>(60, 36), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('V', TVector2D<int>(68, 36), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('W', TVector2D<int>(76, 36), TVector2D<int>(9, 8)));
	FontArray.AddElement(FText('X', TVector2D<int>(86, 36), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText('Y', TVector2D<int>(94, 36), TVector2D<int>(8, 8)));
	FontArray.AddElement(FText('Z', TVector2D<int>(103, 36), TVector2D<int>(7, 8)));
	FontArray.AddElement(FText(' ', TVector2D<int>(88, 16), TVector2D<int>(7, 8)));
	
	TimeUntilRemoval = TimeOnScreen;
	Location = ScreenLocation;
	DisplayText = Text;
	FontSizeMod = FontSize;
}

void TextObject::Tick(float DeltaTime)
{
	if (LifeTime > TimeUntilRemoval)
	{
		GameWorld::GetWorld()->DestroyObject(this);
	}

	if (!DisplayText.empty())
	{
		CreateText(Location, DisplayText, FontSizeMod);
	}
}
