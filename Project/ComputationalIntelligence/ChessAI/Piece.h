#pragma once
#include <vector>

using namespace std;

enum Colour
{
	Black,
	White,
	None
};

enum Type
{
	King = 'K',
	Queen = 'Q',
	Bishop = 'B',
	Knight = 'N',
	Rook = 'R',
	Pawn = 'P',
	Empty = 213
};

struct Position
{
	int x;
	int y;
};

class Piece
{
public:
	Piece(Type type, Colour col = Colour::None);
	~Piece();
//private:
	Colour col;
	Position pos;
	Type piece_type;	
};

