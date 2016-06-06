#pragma once
#include "Piece.h"

#include <iostream>
#include <vector>

using namespace std;

class Board
{
public:
	Board();
	~Board();

	void SetBoard();
	void DrawBoard();
private:
	vector<vector<Piece>> board;
};

