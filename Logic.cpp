#include "Logic.h"
#include <cmath>

namespace Logic
{
	double distance(Coordinate A, Coordinate B)
	{
		return sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
	}
	double distance(Coordinate3D A, Coordinate3D B)
	{
		return sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y) + (A.z-B.z)*(A.z-B.z));
	}
};