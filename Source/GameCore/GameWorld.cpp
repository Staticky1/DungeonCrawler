#include "GameWorld.h"
#include "Objects/BackgroundTile.h"
#include "Objects/PlayerObject.h"
#include "Objects/Camera.h"
#include "Objects/DoorObject.h"
#include "Objects/Enemies/Snake.h"
#include "Objects/TextObject.h"
#include "Objects/HealthDisplay.h"
#include "Objects/WinTrigger.h"
#include "Objects/Enemies/Skeleton.h"
#include "Objects/Enemies/Minotaur.h"

GameWorld* GameWorld::GameWorldPtr = nullptr;

GameWorld::GameWorld()
{
	GameWorldPtr = this;
	srand((int)time(0));
}

GameWorld::~GameWorld()
{
}

bool GameWorld::OnUserCreate()
{
	srand((int)time(0));
	
	

	//Create Tilesheet must happen before any objects are created
	BackgroundTileSheet = new FTileSheet("TileSets/dungeon_tileset_calciumtrice_simple.png");
	WarriorTileSheet = new FTileSheet("TileSets/warrior_spritesheet_calciumtrice.png");
	AttackEffectsTileSheet = new FTileSheet("TileSets/AttackEffects.png");
	SnakeTileSheet = new FTileSheet("TileSets/snake_spritesheet_calciumtrice.png");
	SkeletonTileSheet = new FTileSheet("TileSets/skeleton_spritesheet_calciumtrice.png");
	BoneTileSheet = new FTileSheet("TileSets/BoneProjectile.png");
	MinotaurTileSheet = new FTileSheet("TileSets/minotaur_spritesheet_calciumtrice.png");

	FontTileSheet = new FTileSheet("TileSets/boxy_bold_font.png"); 
	HeartTileSheet = new FTileSheet("TileSets/Heart_Sprite_Sheet.png");

	

	



	return true;
}

bool GameWorld::OnUserUpdate(float fElapsedTime)
{
	if (!bHasGameStarted)
	{
		CreateDungeon();

		Player = CreateGameObject<PlayerObject>(TVector2D<float>(0,0));
		CameraPtr = CreateGameObject<Camera>(TVector2D<float>(0, 0));

		Snake* SnakeObject = CreateGameObject<Snake>(TVector2D<float>(0, 256));
		SnakeObject->bIsHoldingKey = true;

		Skeleton* SkelHP = CreateGameObject<Skeleton>(TVector2D<float>(-288, 288));
		CreateGameObject<Skeleton>(TVector2D<float>(-224, 224));
		SkelHP->bHasHealthPotion = true;


		Skeleton* Skel = CreateGameObject<Skeleton>(TVector2D<float>(-256, 64));
		Skel->bIsHoldingKey = true;

		Snake* SnakeObjectHP = CreateGameObject<Snake>(TVector2D<float>(-256, 464));
		SnakeObjectHP->bHasHealthPotion = true;

		Minotaur* MinotaurPtr = CreateGameObject<Minotaur>(TVector2D<float>(-480, 464));
		MinotaurPtr->bIsHoldingKey = true;

		TextObject* Text = CreateGameObject<TextObject>(TVector2D<float>(0, 0));
		Text->Initalize(TVector2D<float>(75, 50), "WASD TO MOVE", 8.f, 1);

		Text = CreateGameObject<TextObject>(TVector2D<float>(0, 0));
		Text->Initalize(TVector2D<float>(15, 70), "LEFT AND RIGHT MOUSE TO ATTACK", 8.f, 1);


		Text = CreateGameObject<TextObject>(TVector2D<float>(0, 0));
		Text->Initalize(TVector2D<float>(75, 170), "FIND THE EXIT", 8.f, 1);

		HealthDisplay* HealthDisplayObject = CreateGameObject<HealthDisplay>(TVector2D<float>(0, 0));
		HealthDisplayObject->InitalizeDisplay(Player, TVector2D<int>(0, 10));

		CreateGameObject<WinTrigger>(TVector2D<float>(-576, 464));

		bHasGameStarted = true;
		return true;
	}

	TickGroup(PreWorkTickGroup, fElapsedTime);
	
	//Work Step
	TickGroup(WorkTickGroup, fElapsedTime);

	CheckObjectsPendingCreation();

	//Draw Step

	const olc::Pixel BackgroundColor = olc::Pixel();
	Clear(BackgroundColor);

	DrawObjects(fElapsedTime);

	//Post Work

	TickGroup(PostWorkTickGroup, fElapsedTime);

	CheckObjectsPendingDestruction();

	CameraPosition = CameraPtr->WorldPosition;
	
	if (!Player->bIsAlive)
	{

		TextObject* Text = CreateGameObject<TextObject>(TVector2D<float>(0, 0));
		Text->Initalize(TVector2D<float>(70, 110), "YOU DIED", 20.f, 2);
	}

//	std::cout << ObjectsReadyForDestruction.Num() << std::endl;

/*	if (ObjectsReadyForDestruction.Num() > 0)
/	{

		GameObject* Object = ObjectsReadyForDestruction[0];
		ObjectsReadyForDestruction.RemoveIndex(0);
		if (Object->IsReadyForDestruction )
		{
			delete Object;
		}
		
	}*/

	return true;
}

void GameWorld::DestroyObject(GameObject* ObjectForDestruction)
{
	if (ObjectForDestruction)
	{
		if (ObjectsPendingCreation.Contains(ObjectForDestruction))
		{
			//Objects cannot be destroyed before they are created!
			return;
		}


		ObjectsPendingDestruction.AddUnique(ObjectForDestruction);
	}
}

GameWorld* GameWorld::GetWorld()
{
	return GameWorldPtr;
}

void GameWorld::TickGroup(TArray<GameObject*>& Objects, float DeltaTime)
{
	for (int ElementIndex = 0; ElementIndex < Objects.Num(); ElementIndex++)
	{
		GameObject* CurrentObject = Objects[ElementIndex];

		CurrentObject->Tick(DeltaTime);
		CurrentObject->PostTick(DeltaTime);
	}
}

void GameWorld::CheckObjectsPendingCreation()
{
	for (int ElementIndex = 0; ElementIndex < ObjectsPendingCreation.Num(); ElementIndex++)
	{
		GameObject* CurrentObject = ObjectsPendingCreation[ElementIndex];

		CurrentObject->BeginPlay();

		//Move object into tick group
		if (CurrentObject->bTicks)
		{
			switch (CurrentObject->GetTickGroup())
			{
				case ETickGroup::PREWORK:
				{
					PreWorkTickGroup.AddElement(CurrentObject);
					break;
				}
				case ETickGroup::WORKGROUP:
				{
					WorkTickGroup.AddElement(CurrentObject);
					break;
				}
				case ETickGroup::POSTWORK:
				{
					PostWorkTickGroup.AddElement(CurrentObject);
					break;
				}
			}
		}
	}

	if (ObjectsPendingCreation.Num() > 0)
	{
		ObjectsPendingCreation.Clear();
	}
}

void GameWorld::CheckObjectsPendingDestruction()
{
	for (int ElementIndex = 0; ElementIndex < ObjectsPendingDestruction.Num(); ElementIndex++)
	{
		GameObject* CurrentObject = ObjectsPendingDestruction[ElementIndex];

		if (CurrentObject)
		{
			CurrentObject->OnDestruction();

			//Remove from all game objects array
			AllGameObjects.RemoveElement(CurrentObject);

			//remove from ticking groups
			if (CurrentObject->bTicks)
			{
				switch (CurrentObject->GetTickGroup())
				{
				case ETickGroup::PREWORK:
				{
					PreWorkTickGroup.RemoveElement(CurrentObject);
					break;
				}
				case ETickGroup::WORKGROUP:
				{
					WorkTickGroup.RemoveElement(CurrentObject);
					break;
				}
				case ETickGroup::POSTWORK:
				{
					PostWorkTickGroup.RemoveElement(CurrentObject);
					break;
				}
				}
			}
			//ObjectsReadyForDestruction.AddUnique(CurrentObject);
			//CurrentObject->IsReadyForDestruction = true;
			delete CurrentObject;
		}

	}

	ObjectsPendingDestruction.Clear();

}

void GameWorld::DrawObjects(float DeltaTime)
{

	SortArray(AllGameObjects, 0, AllGameObjects.Num() - 1);

	for (int ElementIndex = 0; ElementIndex < AllGameObjects.Num(); ElementIndex++)
	{
		GameObject* CurrentObject = AllGameObjects[ElementIndex];



		if (CurrentObject->bShouldDraw)
		{


			TVector2D<float> ObjectScreenPos = WorldPositionToScreenPosition(CurrentObject->WorldPosition + CurrentObject->SpriteOffset);

			//Don't show objects if they are outside of the screen bounds
			if ((ObjectScreenPos.X > -32 && ObjectScreenPos.X < SCREEN_WIDTH + 32) && (ObjectScreenPos.Y > -32 && ObjectScreenPos.Y < SCREEN_WIDTH + 32))
			{

				int CurrentFrame = 0;

				//gets the current frame of animation to be displayed
				if (CurrentObject->Sprite.AnimationType == EAnimationType::LOOP_FOREVER || CurrentObject->Sprite.AnimationType == EAnimationType::LOOP_ONCE && !CurrentObject->Sprite.bIsAnimationPaused)
				{
					float FramesPerSecond = 1 / CurrentObject->Sprite.AnimationLength;
					CurrentFrame = int(fmod(CurrentObject->Sprite.AnimationPlayTime, CurrentObject->Sprite.NumberOfFrames / FramesPerSecond) / CurrentObject->Sprite.AnimationLength);

					
					
					CurrentObject->Sprite.AnimationPlayTime += DeltaTime;
				}

				if (CurrentObject->Sprite.bIsAnimationPaused)
				{
					CurrentFrame = CurrentObject->Sprite.NumberOfFrames - 1;
				}

				TVector2D<int> SpriteOffset = CurrentObject->Sprite.CalculateFramePosition(CurrentFrame);



				olc::vi2d ScreenPos = olc::vi2d(int(ObjectScreenPos.X), int(ObjectScreenPos.Y));

				/* No sub pixel blending :(
				*
				float WholeX = floor(ObjectScreenPos.X);
				float DecimalX = ObjectScreenPos.X - WholeX;
				float WholeY = floor(ObjectScreenPos.Y);
				float DecimalY = ObjectScreenPos.Y - WholeY;
				*
				//SetSubPixelOffset(DecimalX, DecimalY);
				*/

				olc::vi2d Offset = olc::vi2d(SpriteOffset.X, SpriteOffset.Y);
				olc::vi2d Size = olc::vi2d(CurrentObject->Sprite.SpriteSize.X, CurrentObject->Sprite.SpriteSize.Y);


				SetPixelMode(CurrentObject->Sprite.RequiredPixelMode);

				this->DrawPartialSprite(ScreenPos, CurrentObject->Sprite.TileSheet->Get(), Offset, Size, 1, CurrentObject->Sprite.FlipSprite);

				//Sets stops the animation after one loop
				if (CurrentObject->Sprite.AnimationType != EAnimationType::NONE && CurrentObject->Sprite.AnimationType != EAnimationType::STILL)
				{
					if (CurrentObject->Sprite.AnimationPlayTime > (CurrentObject->Sprite.AnimationLength * CurrentObject->Sprite.NumberOfFrames))
					{
						if (CurrentObject->Sprite.AnimationType == EAnimationType::LOOP_ONCE)
						{
							CurrentObject->Sprite.bIsAnimationPaused = true;
						}

						//Call function because animation is finished
						CurrentObject->OnAnimationFinish();
						//CurrentObject->Sprite.AnimationPlayTime = 0.f;
					}
				}


				//Draw Hitboxes

				if (CurrentObject->ObjectHitBox.bShowHitBoxInGame)
				{
					TVector2D<float> ObjectScreenPos = WorldPositionToScreenPosition(CurrentObject->WorldPosition + CurrentObject->ObjectHitBox.OffsetFromOrign);
					olc::vi2d ScreenPos = olc::vi2d(int(ObjectScreenPos.X), int(ObjectScreenPos.Y));
					olc::vi2d BoxSize = olc::vi2d(int(CurrentObject->ObjectHitBox.Size.X), int(CurrentObject->ObjectHitBox.Size.Y));

					if (CurrentObject->ObjectHitBox.Shape == EHitBoxShape::SQUARE)
					{
						DrawRect(ScreenPos, BoxSize);
					}
					else
					{
						DrawCircle(ScreenPos, BoxSize.x);
					}
					


					
				}
			}
		}
	}
}

void GameWorld::SortArray(TArray<GameObject*>& InArray, int Low, int High)
{
	if (Low < High)
	{
		int Pi = Partition(InArray, Low, High);

		SortArray(InArray, Low, Pi - 1);
		SortArray(InArray, Pi + 1, High);
	}
}

int GameWorld::Partition(TArray<GameObject*>& InArray, int Low, int High)
{
	GameObject* Pivot = InArray[High];

	int Index = Low - 1;

	for (int LoopIndex = Low; LoopIndex <= High - 1; LoopIndex++)
	{
		if (InArray[LoopIndex]->ObjectZHeight < Pivot->ObjectZHeight) 
		{
			Index++;
			InArray.Swap(Index, LoopIndex);
		}
		else if (InArray[LoopIndex]->ObjectZHeight == Pivot->ObjectZHeight && InArray[LoopIndex]->WorldPosition.Y + InArray[LoopIndex]->SpriteOffset.Y - InArray[LoopIndex]->Sprite.SpriteSize.Y > Pivot->WorldPosition.Y + Pivot->SpriteOffset.Y - Pivot->Sprite.SpriteSize.Y)
		{
			Index++;
			InArray.Swap(Index, LoopIndex);
		}
	}

	InArray.Swap(Index + 1, High);
	return Index + 1;
}

TVector2D<float> GameWorld::WorldPositionToScreenPosition(TVector2D<float> WorldPos) const
{
	return (CameraPosition + TVector2D<float>(SCREEN_WIDTH/2,SCREEN_HEIGHT/2))- WorldPos;
}

TVector2D<float> GameWorld::ScreenPositionToWorldPosition(TVector2D<float> ScreenPos) const
{
	return (CameraPosition + TVector2D<float>(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)) - ScreenPos;
}

void GameWorld::CreateRoom(TVector2D<float> RoomLocation, TVector2D<int> RoomSize, TArray<int>& DoorLocations, bool bSpawnDoors, bool bSandyFloor)
{
	
	//Create Floor
	for (int IndexX = (RoomSize.X / 2) * -1 ; IndexX < (RoomSize.X / 2); IndexX++)
	{
		for (int IndexY = (RoomSize.Y / 2) * -1; IndexY < (RoomSize.Y / 2); IndexY++)
		{
			BackgroundTile* BackgroundTileObject = CreateGameObject<BackgroundTile>(TVector2D<float>((IndexX * 16.f),(IndexY * 16.f)) + RoomLocation);
			if (bSandyFloor)
			{
				BackgroundTileObject->SetTileType(ETileType::SANDFLOOR);
			}
			else
			{
				BackgroundTileObject->SetTileType(ETileType::TILEFLOOR);
			}
			
		}
	}

	//Create Wall North
	for (int IndexX = (RoomSize.X / 2) * -1 + 1; IndexX < (RoomSize.X / 2) - 1; IndexX++)
	{
		//Only spawn if wouldnt be blocking a door
		if (DoorLocations[0] < 1 || (IndexX != 0 && IndexX != -1))
		{
			BackgroundTile* BackgroundTileObject = CreateGameObject<BackgroundTile>(TVector2D<float>(IndexX * 16.f,((RoomSize.Y / 2) + 0.5f) * 16.f) + RoomLocation);
			BackgroundTileObject->SetTileType(ETileType::WALL);
		}
		else if (DoorLocations[0] >= 1 && IndexX == 0 && bSpawnDoors)
		{
			//Create door
			DoorObject* NewDoor = CreateGameObject<DoorObject>(TVector2D<float>(IndexX - 0.5f * 16.f, ((RoomSize.Y / 2) + 0.5f) * 16.f) + RoomLocation);
			NewDoor->CreateDoor(0,false);
			if (DoorLocations[0] == 2)
			{
				NewDoor->bIsLocked = true;
			}

		}
	}

	//Wall Corners
	if (RoomSize.X >2)
	{
		BackgroundTile* BackgroundTileObject = CreateGameObject<BackgroundTile>(TVector2D<float>(((RoomSize.X / 2) * -1) * 16.f, ((RoomSize.Y / 2) + 0.5f) * 16.f) + RoomLocation);
		BackgroundTileObject->SetTileType(ETileType::WALL_CORNER_LEFT);

		BackgroundTileObject = CreateGameObject<BackgroundTile>(TVector2D<float>(((RoomSize.X / 2) - 1) * 16.f,((RoomSize.Y / 2) + 0.5f) * 16.f) + RoomLocation);
		BackgroundTileObject->SetTileType(ETileType::WALL_CORNER_RIGHT);
	}

	//Create Ceilings 
	for (int IndexX = (RoomSize.X / 2) * -1; IndexX < (RoomSize.X / 2); IndexX++)
	{
		//South Wall
		if (DoorLocations[2] < 1 || (IndexX != 0 && IndexX != -1))
		{
			if (RoomSize.X > 2)
			{
				BackgroundTile* BackgroundTileObject = CreateGameObject<BackgroundTile>(TVector2D<float>(RoomLocation.X + (IndexX * 16.f), RoomLocation.Y - ((RoomSize.Y / 2) * 16.f)));
				BackgroundTileObject->SetTileType(ETileType::CEILING);
				BackgroundTileObject->Sprite.SpriteSize = TVector2D<int>(16, 25);
			}

		}
		else if (DoorLocations[2] >= 1 && IndexX == 0 && bSpawnDoors)
		{
			//Create door
			DoorObject* NewDoor = CreateGameObject<DoorObject>(TVector2D<float>(RoomLocation.X + (IndexX - 0.5f * 16.f), RoomLocation.Y - ((RoomSize.Y / 2) + 0.5f) * 16.f));
			NewDoor->CreateDoor(0, false);
			if (DoorLocations[2] == 2)
			{
				NewDoor->bIsLocked = true;
			}
		}
	}
	//East Wall
	for (int IndexY = (RoomSize.Y / 2) * -1; IndexY < (RoomSize.Y / 2); IndexY++)
	{
		if (DoorLocations[1] < 1 || (IndexY != 0 && IndexY != -1 && IndexY != -2))
		{
			if (RoomSize.X > 2 || IndexY != RoomSize.Y / 2 -1)
			{
				BackgroundTile* BackgroundTileObject = CreateGameObject<BackgroundTile>(TVector2D<float>(RoomLocation.X - (((RoomSize.X / 2) + 1) * 16.f), RoomLocation.Y - (IndexY * 16.f)));
				BackgroundTileObject->SetTileType(ETileType::CEILING);
			}
		}
		else if (DoorLocations[1] >= 1 && IndexY == 0 && bSpawnDoors)
		{	
			DoorObject* NewDoor = CreateGameObject<DoorObject>(TVector2D<float>(RoomLocation.X - (((RoomSize.X / 2) +0.5f) * 16.f), RoomLocation.Y - ((IndexY - 0.5f) * 16.f)));
			NewDoor->CreateDoor(1, false);
			if (DoorLocations[1] == 2)
			{
				NewDoor->bIsLocked = true;
			}
		}
	}

	//West wall
	for (int IndexY = (RoomSize.Y / 2) * -1; IndexY < (RoomSize.Y / 2); IndexY++)
	{
		if (DoorLocations[3] < 1 || (IndexY != 0 && IndexY != -1 && IndexY != -2))
		{
			if (RoomSize.X > 2 || IndexY != RoomSize.Y / 2 - 1)
			{
				BackgroundTile* BackgroundTileObject = CreateGameObject<BackgroundTile>(TVector2D<float>(RoomLocation.X + (((RoomSize.X / 2)) * 16.f), RoomLocation.Y - (IndexY * 16.f)));
				BackgroundTileObject->SetTileType(ETileType::CEILING);
			}
		}
		else if (DoorLocations[3] >= 1 && IndexY == 0 && bSpawnDoors)
		{
			DoorObject* NewDoor = CreateGameObject<DoorObject>(TVector2D<float>(RoomLocation.X + (((RoomSize.X / 2) + 0.5f) * 16.f), RoomLocation.Y - ((IndexY - 0.5f) * 16.f)));
			NewDoor->CreateDoor(2, false);
			if (DoorLocations[3] == 2)
			{
				NewDoor->bIsLocked = true;
			}
		}
	}

	//Decorations

	//Creates a random amount of decoration objects 
	int DecorationNum = std::rand() % 4;
 
	for (int Index = 0; Index < DecorationNum; Index++)
	{
		BackgroundTile* BackgroundTileObject = CreateGameObject<BackgroundTile>(TVector2D<float>(RoomLocation.X + (((RoomSize.X / 2 - 1) - (std::rand() % (RoomSize.X - 1))) * 16.f), RoomLocation.Y + (((RoomSize.Y / 2 - 1) - (std::rand() % RoomSize.Y - 1) ) * 16.f)));
		BackgroundTileObject->SetTileType(ETileType::DECOR);
	}

}

void GameWorld::CreateDungeon()
{
	TArray<int> Doors = TArray<int>({ 1,0,0,0 });

	//Create Spawn Room

	CreateRoom(TVector2D<float>(0,0), TVector2D<int>(8, 8), Doors);

	Doors = TArray<int>({ 1,0,1,0 });
	CreateRoom(TVector2D<float>(0, 128), TVector2D<int>(2, 8), Doors, false);

	TArray<int> Doors2 = TArray<int>({ 0,2,1,0 });
	CreateRoom(TVector2D<float>(0, 256), TVector2D<int>(8, 8), Doors2, true);

	TArray<int> Doors3 = TArray<int>({ 0,1,0,1 });
	CreateRoom(TVector2D<float>(-128, 256), TVector2D<int>(8, 2), Doors3, false);

	TArray<int> Doors4 = TArray<int>({ 2,0,1,1 });
	CreateRoom(TVector2D<float>(-256, 256), TVector2D<int>(8, 8), Doors4, true);

	TArray<int> Doors5 = TArray<int>({ 1,0,1,0 });
	CreateRoom(TVector2D<float>(-256, 144), TVector2D<int>(2, 6), Doors5, false, true); //Sandy floors

	TArray<int> Doors6 = TArray<int>({ 1,0,0,0 });
	CreateRoom(TVector2D<float>(-256, 64), TVector2D<int>(6, 4), Doors6, false, true); //Sandy floors

	TArray<int> Doors7 = TArray<int>({ 1,0,1,0 });
	CreateRoom(TVector2D<float>(-256, 368), TVector2D<int>(2, 6), Doors7, false);

	TArray<int> Doors8 = TArray<int>({ 0,1,1,0 });
	CreateRoom(TVector2D<float>(-256, 464), TVector2D<int>(4, 6), Doors8, true);

	TArray<int> Doors9 = TArray<int>({ 0,1,0,1 });
	CreateRoom(TVector2D<float>(-336, 464), TVector2D<int>(6, 2), Doors9, false);

	TArray<int> Doors10 = TArray<int>({ 0,2,0,1 });
	CreateRoom(TVector2D<float>(-480, 464), TVector2D<int>(12, 12), Doors10, true, true); //Sandy floors
}

bool GameWorld::CheckForCollision(FHitBox& InHitBox, TVector2D<float> Location, TArray<GameObject*>& ObjectsToIgnore, GameObject* CallingObject, bool bCallOnCollision)
{	
	const TVector2D<float> HitBoxLocation = Location + InHitBox.OffsetFromOrign;

	for (int LoopIndex = 0; LoopIndex < AllGameObjects.Num() - 1; LoopIndex++)
	{
		GameObject* CurrentObject = AllGameObjects[LoopIndex];

		bool bIsIgnored = InHitBox.IgnoredObjects.Contains(AllGameObjects[LoopIndex]);

		if (CallingObject && !bIsIgnored)
		{
			bIsIgnored = CurrentObject->ObjectHitBox.IgnoredObjects.Contains(CallingObject);
		}

		if (CurrentObject->bHasCollision && !ObjectsToIgnore.Contains(CurrentObject) && !bIsIgnored)
		{
			const TVector2D<float> OtherHitBoxLocation = CurrentObject->WorldPosition + CurrentObject->ObjectHitBox.OffsetFromOrign;

			//AABB collsion check
			bool AIsRightOfB = HitBoxLocation.X - InHitBox.Size.X > OtherHitBoxLocation.X;
			bool AIsLeftOfB = HitBoxLocation.X < OtherHitBoxLocation.X - CurrentObject->ObjectHitBox.Size.X;
			bool AIsAboveB = HitBoxLocation.Y - InHitBox.Size.Y > OtherHitBoxLocation.Y;
			bool AIsBelowB = HitBoxLocation.Y < OtherHitBoxLocation.Y - CurrentObject->ObjectHitBox.Size.Y;

			if (!(AIsRightOfB || AIsLeftOfB || AIsAboveB || AIsBelowB))
			{
				//Collision
				//std::cout << "Collision!" << std::endl;

				if (bCallOnCollision)
				{
					CurrentObject->OnCollision(CallingObject, false);
					CallingObject->OnCollision(CurrentObject, true);
				}

				return true;
			}
		}		
	}
	return false;
}
