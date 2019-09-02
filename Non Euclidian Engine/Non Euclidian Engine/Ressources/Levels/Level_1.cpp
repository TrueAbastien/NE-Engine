#include "Level_1.h"
#include "..\..\Ground.h"
#include "..\Objects\TestObj.h"

void Level_1::Load(PObjectVec & objs, PPortalVec & portals, Player & player)
{
	std::shared_ptr<Ground> ground(new Ground());
	ground->scale *= 1.2f;
	objs.push_back(ground);

	
	std::shared_ptr<TestObj> obj(new TestObj("Crate_1.obj", 1.8f));
	obj->pos += Vector3(5.0f, 0, 0);
	objs.push_back(obj);
	

	/*
	std::shared_ptr<TestObj> MazeRoom1(new TestObj("MazeRoom/Maze001.obj", 2.5f));
	MazeRoom1->pos += Vector3(5.0f, 0, 0);
	objs.push_back(MazeRoom1);
	*/

	player.SetPosition(Vector3(0, GH_PLAYER_HEIGHT, 5));
}
