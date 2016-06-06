
#include "Board.h"

#include <iostream>
#include <vector>

#include <fcntl.h>
#include <io.h>
#include <stdio.h>


using namespace std;

/*
void move_piece(Pieces a, int x1, int y1, int x2, int y2, vector<vector<Pieces>>& board)
{
	// Verify this is the piece you are trying to move
	if (board[y1][x1] == a)
	{
		if (board[y2][x2] == Empty)
		{
			board[y1][x1] = Empty;
			board[y2][x2] = a;
		}
	}
}
*/

void main()
{
	Board b = Board();

	b.DrawBoard();

	/*auto board = vector<vector<Pieces>>(8, vector<Pieces>(8, Empty));

	set_board(board);

	draw_board(board);		

	move_piece(Pawn, 1, 1, 1, 2, board);

	draw_board(board);*/

	cin.ignore();
}

// TODO: Get rid of (Char) casts
// TODO: user input for specifying chess move
// TODO: conver from Ne4 notation to values
// TODO: Rudimentary AI
// TODO: Chess moves for each player
// TODO: Different colour pieces

// TODO: Board Class Have funtions attached to the board for doing most of the current actions
// TODO: Piece class : this way we have differentiate between black and white pieces