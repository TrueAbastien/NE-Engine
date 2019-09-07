#include "Level_1.h"
#include "..\..\Ground.h"
#include "..\Objects\TestObj.h"
#include "..\Objects\MazeRoom\Maze.h"

void Level_1::Load(PObjectVec & objs, PPortalVec & portals, Player & player)
{
	Maze M(1);
	M.Apply(objs, portals);


	/*//
	std::shared_ptr<Ground> ground(std::make_shared<Ground>());
	ground->scale *= 1.2f;
	objs.push_back(ground);

	std::array<MazeCase, 5> Cases;
	for (int ii = 0; ii < 5; ii++)
	{
		Cases[ii] = MazeCase(ii + 1);
		Cases[ii].Translate(Vector3(ii*5.0f, 0, 0));
		objs.push_back(Cases[ii].get());
	}
	//*/

	
	/*//
	std::shared_ptr<MazeRoom_1> M1(std::make_shared<MazeRoom_1>());
	M1->pos += Vector3(0, 0, 5.0f);
	M1->euler += Vector3(0, -GH_PI / 2, 0);
	objs.push_back(M1);

	std::shared_ptr<MazeRoom_2> M2(std::make_shared<MazeRoom_2>());
	M2->pos += Vector3(5.0f, 0, 5.0f);
	M2->euler += Vector3(0, -GH_PI / 2, 0);
	objs.push_back(M2);

	std::shared_ptr<MazeRoom_3> M3(std::make_shared<MazeRoom_3>());
	M3->pos += Vector3(5.0f, 0, 0);
	objs.push_back(M3);

	std::shared_ptr<MazeRoom_4> M4(std::make_shared<MazeRoom_4>());
	M4->pos += Vector3(5.0f, 0, -5.0f);
	objs.push_back(M4);

	std::shared_ptr<MazeRoom_5> M5(std::make_shared<MazeRoom_5>());
	M5->pos += Vector3(0, 0, -5.0f);
	objs.push_back(M5);
	//*/
	

	/*//
	std::shared_ptr<TestObj> MazeRoom1(new TestObj("MazeRoom/Maze001.obj", 2.5f));
	MazeRoom1->pos += Vector3(5.0f, 0, 0);
	objs.push_back(MazeRoom1);
	//*/

	player.SetPosition(Vector3(0, GH_PLAYER_HEIGHT, 0));
}
