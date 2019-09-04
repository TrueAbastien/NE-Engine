#include "ColliderMaker.h"

#include <fstream>
#include <sstream>
#include <array>


// Run Application
int ColliderMaker::Run()
{
	// Process Opening // ----------
	if (m_path.empty())
		return -1;
	std::ifstream fInput(this->m_path);
	if (!fInput.is_open())
		return -1;
	std::ofstream fOutput(this->m_path, std::ios::app);


	// Read File // ----------
	std::string line;
	Point3D PointInstance;
	Face3D FaceInstance;
	std::vector<Point3D> PointBuffer;
	std::vector<Face3D> FaceBuffer;

	while (!fInput.eof())
	{
		std::getline(fInput, line);

		// Create Vertices (Points)
		if (line.find("v ") == 0)
		{
			std::stringstream ss(line.c_str() + 2);
			float x, y, z;
			ss >> x >> y >> z;
			PointInstance.x = x;
			PointInstance.y = y;
			PointInstance.z = z;
			PointInstance.Index = PointBuffer.size() + 1;
			PointBuffer.push_back(PointInstance);
		}

		// Create Faces
		if (line.find("f ") == 0)
		{
			// Simple Space Verification
			if (*line.rbegin() == ' ')
				line.pop_back();

			// Count Slashes & Spaces
			int n_slash = 0, n_space = 0;
			for (size_t ii = 1; ii < line.size(); ii++)
			{
				if (line[ii] == '/')
				{
					n_slash++;
					line[ii] = ' ';
				}
				else if (line[ii] == ' ')
				{
					n_space++;
				}
			}

			// Verify kCoeff Integrety
			int kCoeff = n_slash / n_space;
			switch (kCoeff)
			{
			case 0:
			case 1:
			case 2:
				break;
			default:
				return 1;
			}

			// Append Vertex Indexes
			std::stringstream ss(line.c_str() + 2);
			std::vector<int> iBuf;
			int tVal;
			for (int aa = 0;; aa++)
			{
				ss >> tVal;
				if (aa > kCoeff)
					if (tVal == iBuf[iBuf.size() - 1])
						break;
				if (aa % (kCoeff + 1) == 0)
					iBuf.push_back(tVal);
			}
			iBuf.pop_back();

			// Create Face
			std::vector<Point3D> pts;
			for (size_t jj = 0; jj < iBuf.size(); jj++)
			{
				if (iBuf[jj] > PointBuffer.size() || iBuf[jj] < 1)
					return 1;
				pts.push_back(PointBuffer[iBuf[jj] - 1]);
			}
			FaceInstance.pts = pts;
			FaceBuffer.push_back(FaceInstance);
		}

		// Error: Collisions are already Set
		if (line.find("c ") == 0)
		{
			return 1;
		}
	}
	if (PointBuffer.empty() || FaceBuffer.empty())
		return 1;

	
	// Apply Collisions Methods // ----------
	std::vector<std::array<int, 4>> FacesAsIndexesBuffer;
	std::vector<Face3D> NewFaceBuffer;

	switch (m_type)
	{

	// CUBE Method
	case CUBE:

		// Read Min & Max Values
		Point3D ptMin = PointBuffer[0], ptMax = PointBuffer[0];
		for (auto pt : PointBuffer)
		{
			ptMax.x = (pt.x > ptMax.x ? pt.x : ptMax.x);
			ptMax.y = (pt.y > ptMax.y ? pt.y : ptMax.y);
			ptMax.z = (pt.z > ptMax.z ? pt.z : ptMax.z);
			ptMin.x = (pt.x < ptMin.x ? pt.x : ptMin.x);
			ptMin.y = (pt.y < ptMin.y ? pt.y : ptMin.y);
			ptMin.z = (pt.z < ptMin.z ? pt.z : ptMin.z);
		}
		
		// Create All Points
		std::array<std::array<Point3D, 4>, 2> allPts;
		allPts[0][0] = ptMin;
		allPts[1][0] = ptMax;
		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				PointInstance.x = (y == 0 ? allPts[(x + 1) % 2][0].x : allPts[x][0].x);
				PointInstance.y = (y == 1 ? allPts[(x + 1) % 2][0].y : allPts[x][0].y);
				PointInstance.z = (y == 2 ? allPts[(x + 1) % 2][0].z : allPts[x][0].z);
				allPts[x][y + 1] = PointInstance;
			}
		}

		// Fill File
		fOutput << std::endl << std::endl;
		for (int a = 0; a < 2; a++)
		{
			fOutput << std::endl;
			for (int b = 0; b < 4; b++)
			{
				fOutput << "v";
				fOutput << " " << std::to_string(allPts[a][b].x);
				fOutput << " " << std::to_string(allPts[a][b].y);
				fOutput << " " << std::to_string(allPts[a][b].z);
				fOutput << std::endl;
			}
		}
		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				fOutput << std::endl << "c";
				for (int z = 0; z < 3; z++)
					fOutput << " " << std::to_string(PointBuffer.size()
						+ 4 * x + z + (z > 0 ? y : 0) + 1 + (y + z == 4 ? -3 : 0));
			}
			fOutput << std::endl;
		}
		
		break;

	// WALL Method
	case WALL:

		// 'NewFaceBuffer' Construction & Verification
		for (auto F : FaceBuffer)
		{
			for (int ii = 0;; ii)
			{
				if (F.pts.size() == 4)
				{
					NewFaceBuffer.push_back(F);
					break;
				}
				else if (F.pts.size() < 4)
					return 2;
				else if (isLine({ F.pts[ii],
					F.pts[(ii + 1) % F.pts.size()],
					F.pts[(ii + 2) % F.pts.size()] }))
				{
					F.pts.erase(F.pts.begin() + ((ii + 1) % F.pts.size()));
				}
				else ii++;
			}
			if (F.pts.size() != 4)
				return 2;
		}


		// Indexes Buffer Construction
		for (int ii = 0; ii < NewFaceBuffer.size(); ii++)
		{
			FacesAsIndexesBuffer.push_back({});
			for (int jj = 0; jj < 4; jj++)
				FacesAsIndexesBuffer[ii][jj] = NewFaceBuffer[ii].pts[jj].Index;
		}

		// Planes Verification
		std::array<Point3D, 4> tPts;
		for (int ii = 0; ii < NewFaceBuffer.size(); ii++)
		{
			for (int jj = 0; jj < 4; jj++)
				tPts[jj] = NewFaceBuffer[ii].pts[jj];
			if (!isRectPlane(tPts))
				return 2;
		}

		// Fill File
		fOutput << std::endl << std::endl;
		for (int ii = 0; ii < FacesAsIndexesBuffer.size(); ii++)
		{
			fOutput << std::endl << "c";
			for (int jj = 0; jj < 3; jj++)
				fOutput << " " << std::to_string(FacesAsIndexesBuffer[ii][jj]);
		}

		break;

	default:
		break;
	}


	system("pause");
	return 0;
}


// Verify that each Point3D are Aligned
bool ColliderMaker::isLine(std::array<Point3D, 3> pts)
{
	Point3D A, B, C;
	A = pts[0], B = pts[1], C = pts[2];
	Point3D AB, AC;
	AB.x = B.x - A.x; AB.y = B.y - A.y; AB.z = B.z - A.z;
	AC.x = C.x - A.x; AC.y = C.y - A.y; AC.z = C.z - A.z;

	float K;
	if (AB.x)
		K = AC.x / AB.x;
	else if (AB.y)
		K = AC.y / AB.y;
	else if (AB.z)
		K = AC.z / AB.z;
	else
		return true;

	AB.x *= K; AB.y *= K; AB.z *= K;
	return (AB.x == AC.x && AB.y == AC.y && AB.z == AC.z);
}


// Verify that each Point3D given are on same Plane
bool ColliderMaker::isRectPlane(std::array<Point3D, 4> pts)
{
	Point3D A, B, C, D;
	A = pts[0]; B = pts[1]; C = pts[2]; D = pts[3];
	Point3D AB, DC, BC, AD;
	AB.x = B.x - A.x; AB.y = B.y - A.y; AB.z = B.z - A.z;
	DC.x = C.x - D.x; DC.y = C.y - D.y; DC.z = C.z - D.z;
	BC.x = C.x - B.x; BC.y = C.y - B.y; BC.z = C.z - B.z;
	AD.x = D.x - A.x; AD.y = D.y - A.y; AD.z = D.z - A.z;

	bool shouldTrigger = (AB.x == DC.x && AB.y == DC.y && AB.z == DC.z)
		&& (BC.x == AD.x && BC.y == AD.y && BC.z == AD.z);
	if (!shouldTrigger)
		return false;

	float tValue;
	Point3D X, Y;
	std::array<Point3D, 4> allPts = { AB,BC,DC,AD };
	for (int ii = 0; ii < 4; ii++)
	{
		X = allPts[ii]; Y = allPts[(ii + 1) % 4];
		tValue = X.x * Y.x + X.y * Y.y + X.z * Y.z;
		if (tValue != 0.0f)
			return false;
	}

	return true;
}
