#include "Board.h"

Board::Board()
{
	board = vector<vector<Piece>>(8, vector<Piece>(8, Piece(Type::Empty, Colour::None)));
	SetBoard();
}


Board::~Board()
{
}

void Board::SetBoard()
{
	for (auto y = 0; y < 8; ++y)
	{
		if (y == 0)
		{
			// WHITE
			board[y][0] = Piece(Rook, White);
			board[y][1] = Piece(Knight, White);
			board[y][2] = Piece(Bishop, White);
			board[y][3] = Piece(King, White);
			board[y][4] = Piece(Queen, White);
			board[y][5] = Piece(Bishop, White);
			board[y][6] = Piece(Knight, White);
			board[y][7] = Piece(Rook, White);
		}
		else if (y == 7)
		{
			// BLACK
			board[y][0] = Piece(Rook, Black);
			board[y][1] = Piece(Knight, Black);
			board[y][2] = Piece(Bishop, Black);
			board[y][3] = Piece(King, Black);
			board[y][4] = Piece(Queen, Black);
			board[y][5] = Piece(Bishop, Black);
			board[y][6] = Piece(Knight, Black);
			board[y][7] = Piece(Rook, Black);
		}
		else if (y == 1)
		{
			// WHITE
			for (auto x = 0; x < 8; ++x)
			{
				board[y][x] = Piece(Pawn, White);
			}
		}
		else if (y == 6)
		{
			// BLACK
			for (auto x = 0; x < 8; ++x)
			{
				board[y][x] = Piece(Pawn, Black);
			}
		}
	}
}

void Board::DrawBoard()
{
	system("cls");
	cout << "Chess AI Test - V1.0" <<
		endl << "=====================" << endl << endl;

	cout << "   a b c d e f g h" << endl;
	for (auto j = 0; j < 17; ++j)
	{
		if ((j + 1) % 2 == 0)
		{
			cout << " " << j / 2;
		}
		else cout << "  ";
		for (auto i = 0; i < 17; ++i)
		{
			if (j == 0)
			{
				if (i == 0)
					cout << (char)218;
				else if (i == 16) {
					cout << (char)191;
				}
				else if (i % 2 == 0) {
					cout << (char)194;
				}
				else {
					cout << (char)196;
				}
			}
			else if (j == 16)
			{
				if (i == 0)
					cout << (char)192;
				else if (i == 16) {
					cout << (char)217;
				}
				else if (i % 2 == 0) {
					cout << (char)193;
				}
				else {
					cout << (char)196;
				}
			}
			else
			{
				if (j % 2 == 1 && i % 2 != 1)
					cout << (char)179;
				else if (j % 2 == 1)
				{
					// OUTPUT CHARACTERS HERE
					int index_i = i / 2;
					int index_j = j / 2;

					if (board[index_j][index_i].piece_type != Empty)
					{
						cout << (char)board[index_j][index_i].piece_type;
					}
					else  if (index_j % 2 == 0)
					{
						if (index_i % 2 == 0)
							cout << (char)219;
						else cout << " ";
					}
					else
					{
						if (index_i % 2 == 0)
							cout << " ";
						else cout << (char)219;
					}
				}
				if (j % 2 == 0 && i == 0)
					cout << (char)195;
				else if (j % 2 == 0 && i == 16)
					cout << (char)180;
				else if (j % 2 == 0 && i % 2 == 0)
					cout << (char)197;
				else if (j % 2 == 0)
					cout << (char)196;
			}
		}
		cout << endl;
	}
	cout << endl;
}
