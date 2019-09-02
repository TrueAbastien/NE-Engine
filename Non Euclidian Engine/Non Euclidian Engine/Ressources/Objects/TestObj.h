#pragma once
#include "..\..\Object.h"
#include "..\..\Resources.h"

class TestObj : public Object
{
public:
	TestObj(std::string obj, float _scale = 1.0f)
	{
		mesh = AquireMesh(obj.c_str());
		shader = AquireShader("texture");
		texture = AquireTexture("Gold.bmp");
		scale = Vector3(_scale);
	}
	virtual ~TestObj() {}
};