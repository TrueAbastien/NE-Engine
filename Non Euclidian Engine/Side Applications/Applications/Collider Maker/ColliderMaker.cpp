#include "ColliderMaker.h"

#include <fstream>
#include <sstream>

int ColliderMaker::Run()
{
	// Process Opening // ----------
	if (m_path.empty())
		return -1;
	std::fstream file;
	file.open(this->m_path, std::fstream::in | std::fstream::out | std::fstream::app);
	if (!file.is_open())
		return -1;


	// Read File // ----------
	std::string line;
	Point3D PointInstance;
	Face3D FaceInstance;
	std::vector<Point3D> PointBuffer;
	std::vector<Face3D> FaceBuffer;
	unsigned int COUNT = 0;

	while (!file.eof())
	{
		std::getline(file, line);
		COUNT++;

		// Create Vertices (Points)
		if (line.find("v ") == 0)
		{
			std::stringstream ss(line.c_str() + 2);
			float x, y, z;
			ss >> x >> y >> z;
			PointInstance.x = x;
			PointInstance.y = y;
			PointInstance.z = z;
			PointInstance.Index = COUNT;
			PointBuffer.push_back(PointInstance);
		}

		// Create Faces
		if (line.find("f ") == 0)
		{
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

			std::stringstream ss(line.c_str() + 2);
			std::vector<int> iBuf;
			/// TODO: append each value to Buffer

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

			std::vector<Point3D> pts;
			for (size_t jj = 0; jj < iBuf.size(); jj++)
			{
				if (iBuf[jj] > PointBuffer.size() || iBuf[jj] < 1)
					return 1;
				pts.push_back(PointBuffer[iBuf[jj] - 1]);
				jj += kCoeff;
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

	/// TODO

	return 0;
}
