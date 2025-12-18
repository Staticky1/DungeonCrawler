#pragma once

#include "Core.h"
#include "olcPixelGameEngine.h"
#include "GameObject.h"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240
#define PIXEL_SIZE 4

//Game world class
// handles all the main functions of the game loop, calling tick functions and tracking every created object


class GameObject;
class BackgroundTile;
class PlayerObject;
class Camera;

class GameWorld : public olc::PixelGameEngine
{
public:
	GameWorld();
	~GameWorld();

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;


	//Function that creates a gameobject that allows a game object to the created with a world pos
	template <typename T>
	T* CreateGameObject(TVector2D<float> WorldPosition = TVector2D<float>(0.f,0.f));

	//Moves an object to the pending deletion array
	void DestroyObject(GameObject* ObjectForDestruction);

	static GameWorld* GetWorld();

private:

	static GameWorld* GameWorldPtr;


	///////////////////////////////////////
	//
	//			Ticking
	//
	//////////////////////////////

	bool bHasGameStarted = false;

	//Keeps a track of every world object
	TArray<GameObject*> AllGameObjects;

	//Objects that tick during the work step
	TArray<GameObject*> WorkTickGroup;

	TArray<GameObject*> PreWorkTickGroup;

	TArray<GameObject*> PostWorkTickGroup;

	//List of objects that have been created, but are not yet ticking calls the BeginPlay function
	TArray <GameObject*> ObjectsPendingCreation;

	//list of objects to destroy
	TArray <GameObject*> ObjectsPendingDestruction;

	TArray <GameObject*> ObjectsReadyForDestruction;

	void TickGroup(TArray <GameObject*>& Objects, float DeltaTime);

	void CheckObjectsPendingCreation();

	void CheckObjectsPendingDestruction();
	
	///////////////////////////////////////////////////
	//
	//			Drawing the Frame
	//

	//Draws everyobject to the screen
	void DrawObjects(float DeltaTime);

	//the camerapositon represent the world location that is at the center of the screen, when drawing objects there world positions are relative to this location
	TVector2D<float> CameraPosition = TVector2D<float>(0.f, 0.f);



	void SortArray(TArray<GameObject*>& InArray, int Low, int High);
	int Partition(TArray<GameObject*>& InArray, int Low, int High);

public:

	TVector2D<float> WorldPositionToScreenPosition(TVector2D<float> WorldPos) const;
	TVector2D<float> ScreenPositionToWorldPosition(TVector2D<float> ScreenPos) const;

	FTileSheet* BackgroundTileSheet = nullptr;
	FTileSheet* WarriorTileSheet = nullptr;
	FTileSheet* AttackEffectsTileSheet = nullptr;
	FTileSheet* SnakeTileSheet = nullptr;
	FTileSheet* FontTileSheet = nullptr;
	FTileSheet* HeartTileSheet = nullptr;
	FTileSheet* SkeletonTileSheet = nullptr;
	FTileSheet* BoneTileSheet = nullptr;
	FTileSheet* MinotaurTileSheet = nullptr;

	////////////////////////////////////////////////////
	//
	//   Player
	//

	PlayerObject* Player = nullptr;
	Camera* CameraPtr = nullptr;


	/////////////////////////////////////////////////////
	///
	///		Level Creation
	/// 
	/// 

	/*Creates a room of various size
	*
	*  @param1 Center of the room in world space
	* @param2 Size of the room in tiles
	* @param3 If a door is at a locations, Each element being N, E, S, W
	*/ 
	void CreateRoom(TVector2D<float> RoomLocation, TVector2D<int> RoomSize, TArray<int>& DoorLocations, bool bSpawnDoors = true, bool bSandyFloor = false );

	void CreateDungeon();


	////////////////////////////////////////////////////
	///
	///				
	///				Collision 
	/// 
	
	// returns true if there is a collision
	bool CheckForCollision(FHitBox& InHitBox, TVector2D<float> Location, TArray<GameObject*>& ObjectsToIgnore, GameObject* CallingObject = nullptr, bool bCallOnCollision = true);

};

template<typename T>
inline T* GameWorld::CreateGameObject(TVector2D<float> WorldPosition)
{
	T* NewClass = new T();
	GameObject* NewGameObject = static_cast<GameObject*>(NewClass);

	try
	{
		if (NewGameObject)
		{
			AllGameObjects.AddElement(NewClass);
			ObjectsPendingCreation.AddElement(NewClass);

			NewGameObject->WorldPosition = WorldPosition;

			return NewClass;
		}
		else
		{
			throw 101;
		}
	}
	catch (...)
	{
		std::cout << "ERROR: Attempted to create a non-gameobject!!!";
		delete NewClass;
		return nullptr;
	}
}
