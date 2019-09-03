#pragma once
#include "..\Application.h"

#include <string>
#include <vector>

class ColliderMaker : public Application
{
public:

	enum ProcessingType
	{
		CUBE = 0,
		WALL = 1
	};

	ColliderMaker(std::string path = "", int type = CUBE)
		: m_path(path), m_type(type) {}
	int Run() override;

	struct Point3D { float x, y, z; int Index; };
	struct Face3D { std::vector<Point3D> pts; };

private:

	bool isPlaneQuad(std::array<Point3D, 4> pts);

	std::string m_path;
	int m_type;
};