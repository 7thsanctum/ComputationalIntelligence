#pragma once
#include <string>

class Point
{
public:
	int x;
	int y;
	int z;
	Point(void);
	~Point(void);

	std::string GetPointText();
	static double distance(int x1, int y1, int x2, int y2);
	static double distance(int x1, int y1, int z1, int x2, int y2, int z2);
};

