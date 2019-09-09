#pragma once
#include "MazeRoom.h"
#include "..\..\..\Portal.h"
#include <array>

class MazeCase
{
public:
	MazeCase(int id = 0, int section = 0, int X = 0, int Y = 0);
	std::shared_ptr<MazeRoom> get() const;
	void set(int id, int R);
	void _set(std::array<bool, 4> composition);
	void Rotate(int R);
	void Translate(Vector3 T);

	int Section = 0;
	int X = 0, Y = 0;

	std::array<bool, 4> NESW() const { return DecypherID(); }
	bool Up() const { return DecypherID()[0]; }
	bool Right() const { return DecypherID()[1]; }
	bool Down() const { return DecypherID()[2]; }
	bool Left() const { return DecypherID()[3]; }

	void DebugPrint(int Id);

private:
	std::shared_ptr<MazeRoom> m_room;
	Vector3 m_pos;
	int m_Ry;

	std::array<bool, 4> DecypherID() const;
};
typedef std::vector<std::shared_ptr<MazeCase>> PMazeVec;


class Maze
{
public:
	Maze(const unsigned int seed, int nSection = 4, int width = 4, int height = 4, int nJonction = 2);
	void Apply(PObjectVec& Objects, PPortalVec& Portals);

	const int amountOfSection, SectionWidth, SectionHeight;

private:
	PMazeVec m_Rooms;
	PPortalVec m_Portals;
};