#pragma once
#include "Vector.h"
#include <string>
#include <iostream>

class Material
{
public:
	Material(std::string _name = "__Default", Vector3 Ka = Vector3(0.0f), Vector3 Kd = Vector3(1.0f), Vector3 Ks = Vector3(1.0f), float Ns = 0.0f)
		: name(_name), ambient(Ka), diffuse(Kd), specular(Ks), shininess(Ns)
	{}

	std::string name;

	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	float shininess;

	Material lookFor(std::string _name)
	{
		if (name == _name)
			return *this;
		return Material("_Error");
	}
	bool isError()
	{
		return (this->name == "_Error");
	}

	void SetMaterial()
	{
		// TODO
	}

	//DEBUG
	void Print()
	{
		std::cout << name << '\n';
		std::cout << ambient.x << ' ' << ambient.y << ' ' << ambient.z << '\n';
		std::cout << diffuse.x << ' ' << diffuse.y << ' ' << diffuse.z << '\n';
		std::cout << specular.x << ' ' << specular.y << ' ' << specular.z << '\n';
		std::cout << shininess << '\n';
	}
};