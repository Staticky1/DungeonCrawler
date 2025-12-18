#define OLC_PGE_APPLICATION

#include "GameCore/Core.h"
#include "GameCore/GameWorld.h"





int main()
{

	GameWorld World;
	if (World.Construct(SCREEN_WIDTH, SCREEN_HEIGHT, PIXEL_SIZE, PIXEL_SIZE))
	{
		World.Start();
	}

	return 0;
}