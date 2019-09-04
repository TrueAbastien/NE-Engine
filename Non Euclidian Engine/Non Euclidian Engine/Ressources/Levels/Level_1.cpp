#include "Level_1.h"
#include "..\..\Ground.h"
#include "..\Objects\TestObj.h"

void Level_1::Load(PObjectVec & objs, PPortalVec & portals, Player & player)
{
	std::shared_ptr<Ground> ground(std::make_shared<Ground>());
	ground->scale *= 1.2f;
	objs.push_back(ground);

	
	std::shared_ptr<TestObj> crate(std::make_shared<TestObj>("TestCrate.obj", 1.8f));
	crate->pos += Vector3(5.0f, 0, 0);
	objs.push_back(crate);
	
	std::shared_ptr<TestObj> maze(std::make_shared<TestObj>("TestMaze.obj", 2.0f, "Bush.bmp"));
	maze->pos += Vector3(0, 0, 5.0f);
	maze->euler += Vector3(4.71f, 0, 0);
	objs.push_back(maze);
	

	/*
	std::shared_ptr<TestObj> MazeRoom1(new TestObj("MazeRoom/Maze001.obj", 2.5f));
	MazeRoom1->pos += Vector3(5.0f, 0, 0);
	objs.push_back(MazeRoom1);
	*/

	player.SetPosition(Vector3(0, GH_PLAYER_HEIGHT, 5));
}
