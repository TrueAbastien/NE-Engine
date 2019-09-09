#include "Maze.h"
#include "..\..\..\Ground.h"

#include <iostream>

#define F_DIR_X(D) -pow(D, 3)/3.0f + pow(D, 2)*2.0f - D*8.0f/3.0f
#define F_DIR_Y(D) pow(D, 3)/3.0f - pow(D, 2) - D/3.0f + 1.0f


// MazeCase: Create Maze
MazeCase::MazeCase(int Index, int section, int x, int y)
	: Section(section), X(x), Y(y)
{
	this->set(Index, 0);
	m_pos = m_room->pos;
}


// MazeCase: Return Room
std::shared_ptr<MazeRoom> MazeCase::get() const
{
	m_room->euler.y = float(m_Ry) * GH_PI / 2;
	//std::cout << m_room->euler.y << std::endl; //DEBUG
	m_room->pos = m_pos;
	return m_room;
}


// MazeCase: Reset MazeRoom relative Data
void MazeCase::set(int id, int R)
{
	switch (id)
	{
	case 1: m_room = std::make_shared<MazeRoom_1>(); break;
	case 2: m_room = std::make_shared<MazeRoom_2>(); break;
	case 3: m_room = std::make_shared<MazeRoom_3>(); break;
	case 4: m_room = std::make_shared<MazeRoom_4>(); break;
	case 5: m_room = std::make_shared<MazeRoom_5>(); break;
	default: m_room = std::make_shared<MazeRoom>(); break;
	}
	m_Ry = 0;
	this->Rotate(R);
}


// MazeCase: Set MazeRoom Properties to Match Composition
void MazeCase::_set(std::array<bool, 4> composition)
{
	std::vector<std::array<int, 2>> Possibilities;
	for (int ii = 0; ii < 5; ii++)
		for (int jj = 0; jj < 4; jj++)
			Possibilities.push_back({ ii + 1, jj });
	MazeCase Instance;
	for (auto P : Possibilities)
	{
		Instance.set(P[0], P[1]);
		if (Instance.NESW() == composition)
		{
			this->set(P[0], P[1]);
			break;
		}
	}
}


// MazeCase: Rotate Room
void MazeCase::Rotate(int R)
{
	m_Ry += R < 0 ? -(-R % 4) : R % 4;
}


// MazeCase: Translate Room
void MazeCase::Translate(Vector3 T)
{
	m_pos += T;
}


// MazeCase: Print Debug Information
void MazeCase::DebugPrint(int Id)
{
#ifdef _DEBUG
	std::cout << std::endl << Id << " " << m_Ry << ": ";
#endif
}


// MazeCase: Decypher Room ID
std::array<bool, 4> MazeCase::DecypherID() const
{
	int tVal = m_room->ID();
	std::array<bool, 4> temp, res;
	for (int ii = 3; ii >= 0; ii--)
	{
		temp[ii] = tVal >= pow(2, ii);
		if (temp[ii])
			tVal -= (int)pow(2, ii);
	}
	for (int jj = 0; jj < 4; jj++)
		res[jj] = temp[(jj - m_Ry + 4) % 4];

	//std::cout << std::endl; for (auto V : res) std::cout << V << " "; //DEBUG

	return res;
}


// ---------- ---------- ---------- ---------- ---------- //


// Maze: Constructor
Maze::Maze(const unsigned int seed, int nS, int W, int H, int nJ)
	: amountOfSection(nS < 2 ? 2 : nS)
	, SectionWidth(W < 2 ? 2 : W)
	, SectionHeight(H < 2 ? 2 : H)
{
	// Pre-Generation
	std::srand(seed);
	std::vector<std::vector<PMazeVec>> Board;
	for (int S = 0; S < amountOfSection; S++)
	{
		Board.push_back({});
		for (int Y = 0; Y < SectionHeight; Y++)
		{
			Board[S].push_back({});
			for (int X = 0; X < SectionWidth; X++)
				Board[S][Y].push_back(std::make_shared<MazeCase>(((int)std::rand() % 5) + 1, S, X, Y));
		}
	}

	// Possibilities Construction
	std::vector<std::array<int, 2>> Possibilities;
	std::vector<std::array<int, 2>> PosInstance;
	for (int ii = 0; ii < 5; ii++)
		for (int jj = 0; jj < 4; jj++)
			Possibilities.push_back({ ii + 1, jj });

	/// DEBUG ///
	/*//
	MazeCase Inst;
	for (auto P : Possibilities)
	{
		Inst.set(P[0], P[1]);
		Inst.DebugPrint(P[0]);
		for (int ii = 0; ii < 4; ii++)
			std::cout << Inst.NESW()[ii] << " ";
	}
	std::cout << std::endl;
	//*/

	// Ensure Validity of Each Cases in Maze
	std::shared_ptr<MazeCase> RoomInstance;
	std::array<int, 2> IndexesInstance;
	int tIndex;
	for (int S = 0; S < amountOfSection; S++)
		for (int Y = 0; Y < SectionHeight; Y++)
			for (int X = 0; X < SectionWidth; X++)
			{
				PosInstance = Possibilities;
				RoomInstance = Board[S][Y][X];
				while (true)
				{
					if (PosInstance.empty())
					{
						RoomInstance->set(0, 0);
						break;
					}
					tIndex = std::rand() % PosInstance.size();
					IndexesInstance = PosInstance[tIndex];
					PosInstance.erase(PosInstance.begin() + tIndex);

					if ((X == 0 && RoomInstance->Left())
						|| (Y == 0 && RoomInstance->Down())
						|| (X == SectionWidth - 1 && RoomInstance->Right())
						|| (Y == SectionHeight - 1 && RoomInstance->Up()))
					{
						RoomInstance->set(IndexesInstance[0], IndexesInstance[1]);
						continue;
					}

					if (X != 0)
						if (Board[S][Y][X - 1]->Right() != RoomInstance->Left())
						{
							RoomInstance->set(IndexesInstance[0], IndexesInstance[1]);
							continue;
						}
					if (Y != 0)
						if (Board[S][Y - 1][X]->Up() != RoomInstance->Down())
						{
							RoomInstance->set(IndexesInstance[0], IndexesInstance[1]);
							continue;
						}
					break;
				}
			}

	/// DEBUG
	for (int S = 0; S < Board.size(); S++)
	{
		std::cout << std::endl << "Section: " << S << std::endl;
		for (int Y = 0; Y < Board[S].size(); Y++)
		{
			for (int X = 0; X < Board[S][Y].size(); X++)
				std::cout << Board[S][Y][X]->get() << " ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	//*//
	// Find and Count the Amount of Paths after Correction
	// Then, Join Each Path as One
	PMazeVec MazeInstance, CaseTreeInstance;
	int tCount;
	std::vector<PMazeVec> AllPaths;
	std::vector<std::array<std::pair<std::shared_ptr<MazeCase>, int>, 2>> AllNeighbors;
	for (int S = 0; S < amountOfSection; S++)
	{
		AllPaths.clear();
		MazeInstance.clear();
		for (int Y = 0; Y < SectionHeight; Y++)
			for (int X = 0; X < SectionWidth; X++)
				if (Board[S][Y][X])
					if (Board[S][Y][X]->get())
						if (Board[S][Y][X]->get()->ID() != 0)
							MazeInstance.push_back(Board[S][Y][X]);

		// Loop while MazeInstance is not Empty
		while (!MazeInstance.empty())
		{
			AllPaths.push_back({});
			CaseTreeInstance.clear();
			CaseTreeInstance.push_back(MazeInstance[0]);
			MazeInstance.erase(MazeInstance.begin());
			tCount = 1;

			// Loop while Path Finding is Viable
			while (tCount != 0)
			{
				tCount = 0;

				// Read Each MazeCase in Tree
				for (auto Case : CaseTreeInstance)
				{
					tCount++;
					auto NESW = Case->NESW();

					// Read Each Direction
					for (int D = 0; D < 4; D++)
					{
						if (NESW[D])

							// Look for MazeCase Next to Instance in Specified Direction
							for (unsigned int Index = 0; Index < MazeInstance.size(); Index)
							{
								if (MazeInstance[Index]->X == Case->X - int(F_DIR_X((float)D))
									&& MazeInstance[Index]->Y == Case->Y - int(F_DIR_Y((float)D)))
								{
									CaseTreeInstance.push_back(MazeInstance[Index]);
									MazeInstance.erase(MazeInstance.begin() + Index);
									break;
								}
								else Index++;
							}
					}
				}
				if (tCount != 0)
					for (int Range = 0; Range < tCount; Range++)
					{
						AllPaths.rbegin()->push_back(CaseTreeInstance[0]);
						CaseTreeInstance.erase(CaseTreeInstance.begin());
						std::cout << " " << S << " " << AllPaths.size() << " " << MazeInstance.size() << std::endl; //DEBUG
					}
			}

			std::cout << "  " << S << " " << AllPaths.size() << " " << MazeInstance.size() << std::endl; //DEBUG
		}

		/// DEBUG
		std::cout << "NB Paths: " << AllPaths.size() << std::endl << std::endl;
		for (auto P : AllPaths)
		{
			std::cout << P.size() << std::endl;
			for (auto C : P)
				std::cout << C->X << " " << C->Y << std::endl;
			std::cout << std::endl;
		}

		/*/
		// Determine if two Cases from Different Paths are Close to Each other
		const int amountOfPath = AllPaths.size();
		if (amountOfPath > 1)
		{
			for (int ii = 0; ii < amountOfPath - 1; ii++)
				for (int jj = ii + 1; jj < amountOfPath; jj++)
					for (auto C1 : AllPaths[ii])
						for (auto C2 : AllPaths[jj])
							if (pow(float(C1->X - C2->X + C1->Y - C2->Y), 2) == 1.0f)
								AllNeighbors.push_back({ std::pair<std::shared_ptr<MazeCase>, int>{C1,ii}, {C2,jj} });
		}

		// Shuffle Vector of Neighbors Randomly
		auto RandomNeighbors = AllNeighbors;
		int tIndex;
		if (amountOfPath > 1)
		{
			RandomNeighbors.clear();
			while (!AllNeighbors.empty())
			{
				tIndex = std::rand() % AllNeighbors.size();
				RandomNeighbors.push_back(AllNeighbors[tIndex]);
				AllNeighbors.erase(AllNeighbors.begin() + tIndex);
			}
		}

		// Chose Pair of Cases to Join Paths
		std::vector<int> AllPathIndexes;
		int V1, V2;
		if (amountOfPath > 1)
		{
			for (int a = 0; a < amountOfPath; a++)
				AllPathIndexes.push_back(a);

			for (unsigned int ii = 0; ii < AllPathIndexes.size() - 1; ii++)
				for (unsigned int jj = ii + 1; jj < AllPathIndexes.size(); jj++)
				{
					V1 = AllPathIndexes[ii];
					V2 = AllPathIndexes[jj];

					if (V1 != V2)
						for (auto N : RandomNeighbors)
							if ((N[0].second == V1 && N[1].second == V2)
								|| (N[1].second == V1 && N[0].second == V2))
							{
								for (int Dir = 0; Dir < 4; Dir++)
									if (N[0].first->X == N[1].first->X + int(F_DIR_X((float)Dir))
										&& N[0].first->Y == N[1].first->Y + int(F_DIR_Y((float)Dir)))
									{
										auto Boussole1 = N[0].first->NESW(), Boussole2 = N[1].first->NESW();
										Boussole1[(Dir + 2) % 4] = true;
										Boussole2[Dir] = true;
										N[0].first->_set(Boussole1);
										N[1].first->_set(Boussole2);
										break;
									}
								AllPathIndexes.erase(AllPathIndexes.begin() + jj);
								jj--;
								break;
							}
				}
		}
		//*/
	}
	//*/

	// Create Portal Jonction between Sections
	int nJonction = (nJ < nS ? (nJ < 2 ? (nS == 2 ? 1 : 2) : nJ) : nS - 1);
	/// TODO

	// Set Content Members
	for (int S = 0; S < nS; S++)
		for (int Y = 0; Y < H; Y++)
			for (int X = 0; X < W; X++)
				if (Board[S][Y][X]->get()->ID() != 0)
					m_Rooms.push_back(Board[S][Y][X]);
}


// Maze: Apply and Push each Objects/Portals to Parameters
void Maze::Apply(PObjectVec & Objects, PPortalVec & Portals)
{
	// Push Grounds
	for (int ii = 0; ii < amountOfSection; ii++)
	{
		auto Floor = std::make_shared<Ground>();
		Floor->scale = Vector3(SectionWidth * 2.5f, 1.0f, SectionHeight * 2.5f);
		Floor->pos += Vector3((SectionWidth + 1) * ii * 5.0f + (SectionWidth - 1) * 2.5f, 0, (SectionWidth - 1) * 2.5f);
		Objects.push_back(Floor);
	}

	// Push Rooms
	for (auto M : m_Rooms)
	{
		M->Translate(Vector3(5.0f * M->X, 0, 5.0f * M->Y));
		M->Translate(Vector3(M->Section * 5.0f * (SectionWidth + 1), 0, 0));
		Objects.push_back(M->get());
	}

	// Push Portals
	/// TODO
}
