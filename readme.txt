A university project to build a dungeon crawler styled game in C++. with the restriction of not being allowed to use the vector.h library or any smart pointers. 

Gameplay loop logic is found inside the GameWorld class

All spawned classes in the game are children of GameObject class

PawnObject contains all the logic for Moveable characters/enemies etc

//Known bugs
Killing enemies up against the north wall will sometimes cause their drops to be unreachable
enemies can very rarely clip through the corners of walls and warp away
decorations spawn out of bounds 

TimerObject is not used

//Credits

Based on OneLoneCoder Pixel Game Engine v2.22 - (OLC-3) 

Dungeon tileset, Warrior Tileset, Snake Tileset, Skeleton Tileset and Minotaur Tileset by Calciumtric, usable under Creative Commons Attribution 3.0 license.
Boxy Font by Clint Bellanger, used under CC0

All other art by Me (Josh Jamieson)
