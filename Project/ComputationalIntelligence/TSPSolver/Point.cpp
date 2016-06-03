#include "Point.h"
#include <sstream>

Point::Point(void)
{
}


Point::~Point(void)
{
}

std::string Point::GetPointText()
{
	std::string text;
	std::ostringstream converter;

	converter << x;
	text += converter.str() + std::string(", "); 
	converter.str("");
	converter.clear();
	converter << y;
	text += converter.str() + std::string(", "); 
	//converter << z;
	//text += converter.str(); 
	return text;
}

double Point::distance(int x1, int y1, int x2, int y2)
{
	int x = x2 - x1;
	int y = y2 - y1;

	return sqrt((x*x) + (y*y));
}

double Point::distance(int x1, int y1, int z1, int x2, int y2, int z2)
{
	int x = x2 - x1;
	int y = y2 - y1;
	int z = z2 - z1;

	return sqrt((x*x) + (y*y) + (z*z));
}