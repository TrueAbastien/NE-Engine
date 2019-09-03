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
		CORRIDOR = 1
	};

	ColliderMaker(std::string path = "", ProcessingType type = CUBE)
		: m_path(path), m_type(type) {}
	int Run() override;

	struct Point3D { float x, y, z; unsigned int Index; };
	struct Face3D { std::vector<Point3D> pts; };

private:

	std::string m_path;
	ProcessingType m_type;
};