#include "Applications/Collider Maker/ColliderMaker.h"
using namespace std;

#define CURRENT_APP ColliderMaker
#define PARAMS_AS "Objects/Maze005.obj", ColliderMaker::ProcessingType::WALL

#include <sstream>
#include <iostream>

int main(void)
{
	CURRENT_APP app(PARAMS_AS);
	return app.Run();
}