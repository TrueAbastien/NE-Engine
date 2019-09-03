#include "Applications/Collider Maker/ColliderMaker.h"
using namespace std;

#define CURRENT_APP ColliderMaker
#define PARAMS_AS "TestCrate.obj", ColliderMaker::ProcessingType::CUBE

#include <sstream>
#include <iostream>

int main(void)
{
	CURRENT_APP app(PARAMS_AS);
	return app.Run();
}