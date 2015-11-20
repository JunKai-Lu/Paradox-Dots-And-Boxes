#include "stdafx.h"
#include "GameDefine.h"
#include "windows.h"
#include <iostream>

using namespace std;


ChessBoard::ChessBoard()
{
	for (sint y = 0; y < LEN; y++)
	{
		for (sint x = 0; x < LEN; x++)
		{
			if (OddNum(x) && OddNum(y))
				board[x][y] = BOX;
			else if (EvenNum(x) && EvenNum(y))
				board[x][y] = DOT;
			else
				board[x][y] = EDGE;
		}
	}
}
ChessBoard::ChessBoard(ChessBoardArray &cb_array)
{
	SetChessBoard(cb_array);
}
ChessBoard::ChessBoard(ChessBoardArray &cb_array, Move &move)
{
	//copy the chessboard first then make one move in order to create a new state.
	SetChessBoard(cb_array);
	GameMove(move, false);
}
void ChessBoard::GameMove(Move &move, bool show_msg)
{
	/*
	CAUTION: if edge (x,y) and the boxes near by it have been captured, the function will change the boxes' belonger.  
	*/
	if (DEBUG)
	{
		if (board[move.x][move.y] != EDGE)
		{
			cout << "WARNING [Wrong Edge Location]" << endl;
			system("pause");
		}
	}

	//if 'ShowMsg' is true then sent message.
	if (show_msg)
		GameMoveMsg(move);

	//change the value of the location of board.
	board[move.x][move.y] = move.player;
	
	//check if there is any box is completed.

	if (OddNum(move.x)&&EvenNum(move.y))//horizonal
	{
		if (move.y + 1 < LEN - 1)
			if (GetBoxCompleted(move.x, move.y + 1))
				board[move.x][move.y + 1] = move.player*2;
		if (move.y - 1 > 0)
			if (GetBoxCompleted(move.x, move.y - 1))
				board[move.x][move.y - 1] = move.player*2;	
	}
	else if (OddNum(move.y) && EvenNum(move.x))//vertical
	{
		if (move.x + 1 < LEN - 1)
			if (GetBoxCompleted(move.x + 1, move.y))
				board[move.x + 1][move.y] = move.player*2;
		if (move.x - 1 > 0)
			if (GetBoxCompleted(move.x - 1, move.y))
				board[move.x - 1][move.y] = move.player*2;
	}
	else
	{
		cout << "WARNING: [Wrong move]" << endl;
		system("pause");
	}
	

}
void ChessBoard::GameMoveMsg(Move &m)
{
	std::cout << "===== Move Message =====" << std::endl;
	std::cout << "Msg: ";
	if (m.player == RED)
		Cprintf("Red", 12);
	else
		Cprintf("Blue ", 9);
	std::cout << " Captual Edge";
	Cprintf("(", 10);
	CprintfNum(m.x, 10);
	Cprintf(",", 10);
	CprintfNum(m.y, 10);
	Cprintf(")\n", 10);
	std::cout << "===== Move Message =====" << std::endl;
}
void ChessBoard::SetChessBoard(ChessBoardArray &source)
{
	for (int i = 0; i < LEN; i++)
		for (int j = 0; j < LEN; j++)
			board[i][j] = source[i][j];
}
sint ChessBoard::Winner() const
{
	int red = 0;
	int blue = 0;
	for (int i = 0; i < LEN; i++)
	{
		for (int j = 0; j < LEN; j++)
		{
			if (board[i][j] == RED_BOX)
				red++;
			if (board[i][j] == BLUE_BOX)
				blue++;
		}
	}
	if (red + blue >= BOXNUM)
	{
		if (red > blue)
			return RED;
		else
			return BLUE;
	}
	return 0;
}
sint ChessBoard::ComputeWinner(sint next_player)
{
	if (ExistMoveWithBoas() == false)
	{
		//the outcome of this game is already doomed.
		
		//here we need to invoke GameSolver.
		if (Winner() != 0)
			return Winner();
		return RED;
		//
	}
	return 0;//the outcome of this game is still unpredictable.
}
void ChessBoard::PrintCB() const
{
	//Print Chess ChessBoardArray
	cout << "  ";
	for (int i = 0; i <= (LEN - 1); i++)
	{
		printf("%2d", i);
	}
	cout << "\n";
	for (sint j = 0; j<LEN; j++)
	{
		printf("%2d", j);
		for (sint i = 0; i<LEN; i++)
		{
			if (board[i][j] == BOX)
			{
				Cprintf("  ", 8);
			}
			else if (board[i][j] == RED_BOX)
			{
				Cprintf("¡ö", 12);
			}
			else if (board[i][j] == BLUE_BOX)
			{
				Cprintf("¡ö", 9);
			}
			else if (board[i][j] == EDGE)
			{
				if (OddNum(j) && EvenNum(i))
				{
					cout << "  ";
					//cprintf("©§",15);
				}
				else
				{
					cout << "  ";
					//cprintf("©¥",15);
				}
			}
			else if (board[i][j] == RED)
			{
				if (OddNum(j) && EvenNum(i))
				{
					Cprintf("©§", 12);
				}
				else
				{
					Cprintf("©¥", 12);
				}
			}
			else if (board[i][j] == BLUE)
			{
				if (OddNum(j) && EvenNum(i))
				{
					Cprintf("©§", 9);
				}
				else
				{
					Cprintf("©¥", 9);
				}
			}
			else if (board[i][j] == EMPTY)
			{
				Cprintf("¡ð", 15);
			}
		}
		printf("%2d", j);
		cout << "\n";
	}
	cout << "  ";
	for (int i = 0; i <= (LEN - 1); i++)
	{
		printf("%2d", i);
	}
	cout << "\n";
}

//Advanced function for move

bool ChessBoard::EdgeCauseDeadChain(sint x,sint y,sint box_x, sint box_y) const
{
	if (GetBoxLiberties(box_x, box_y) == CHAIN_BOX)//this box shold be CHAIN BOX at first
	{
		int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
		for (int n = 0; n < 4; n++)
		{
			int edge_x = box_x + Dir[n][0];
			int edge_y = box_y + Dir[n][1];
			int next_box_x = box_x + Dir[n][0] + Dir[n][0];//the actual address of next box.
			int next_box_y = box_y + Dir[n][1] + Dir[n][1];
			if (edge_x != x || edge_y != y)
			{
				if (board[edge_x][edge_y] == EDGE&&next_box_x >= 1 && next_box_x <= LEN - 2 && next_box_y >= 1 && next_box_y <= LEN - 2)
				{
					if (GetBoxLiberties(next_box_x, next_box_y) == CHAIN_BOX)
						return true;
				}
			}
		}
	}
	return false;
}
sint ChessBoard::GetMovesWithBias(Move moves[MOVENUM], sint player)const
{
	//argument 'Moves' is an 'Move' array which can conclude all possible move. we define its size as the number of all edge.
	//this function is mainly used to create random move.
	//this function is const

	int move_num = 0;
	for (sint y = 0; y < LEN; y += 2)
	{
		for (sint x = 1; x < LEN; x += 2)
		{
			//in each iterator we check (x,y) and (y,x).
			if (board[x][y] == EDGE)//that means (x,y) is an empty edge
			{
				if (
					(y == 0 && !EdgeCauseDeadChain(x,y,x, y + 1)) ||
					(y == LEN - 1 && !EdgeCauseDeadChain(x,y,x, y - 1)) ||
					(y>0 && y < LEN - 1 && !EdgeCauseDeadChain(x,y,x, y + 1) && !EdgeCauseDeadChain(x,y,x, y - 1))
					)
				{
					//if the conditions are met, the edge is an available move.
					//save the move.

					moves[move_num].Set(x, y, player);	//save the move to 'Move' array.
					move_num++;							//increase the number of move
				}
			}


			if (board[y][x] == EDGE)
			{
				//we assume there is no any dead box need to be capture.
				//NOTICE: x is always odd number and y is allways even num, so the edge (y,x) is always a vertical edge.
				//thus we only need check box( y+1,x ) and box(y-1,x).

				if (
					(y == 0 && !EdgeCauseDeadChain(y , x, y + 1, x)) ||
					(y == LEN - 1 && !EdgeCauseDeadChain(y , x, y - 1, x)) ||
					(y>0 && y < LEN - 1 && !EdgeCauseDeadChain(y , x, y + 1, x) && !EdgeCauseDeadChain(y , x, y - 1, x))
					)
				{
					//if the conditions are met, the edge is an available move.
					//save the move.

					moves[move_num].Set(y, x, player);	//save the move to 'Move' array.
					move_num++;							//increase the number of move
				}
			}
		}
	}
	return move_num;
}
bool ChessBoard::ExistMoveWithBoas()
{
	for (sint y = 0; y < LEN; y += 2)
	{
		for (sint x = 1; x < LEN; x += 2)
		{
			if (board[x][y] == EDGE)
			{
				if ((y == 0 && !EdgeCauseDeadChain(x, y, x, y + 1)) ||(y == LEN - 1 && !EdgeCauseDeadChain(x, y, x, y - 1)) ||(y>0 && y < LEN - 1 && !EdgeCauseDeadChain(x, y, x, y + 1) && !EdgeCauseDeadChain(x, y, x, y - 1)))
				{
					return true;
				}
			}

			if (board[y][x] == EDGE)
			{
				if ((y == 0 && !EdgeCauseDeadChain(y, x, y + 1, x)) ||(y == LEN - 1 && !EdgeCauseDeadChain(y, x, y - 1, x)) ||(y>0 && y < LEN - 1 && !EdgeCauseDeadChain(y, x, y + 1, x) && !EdgeCauseDeadChain(y, x, y - 1, x)))
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool ChessBoard::CaptureDeadBox(sint player, bool show_msg)
{
	for (sint y = 1; y < LEN - 1; y += 2)
	{
		for (sint x = 1; x < LEN - 1; x += 2)
		{
			if (GetBoxLiberties(x, y) == DEAD_BOX)
			{
				Move move;
				if (board[x + 1][y] == EDGE){ move.Set(x + 1, y, player); }
				if (board[x - 1][y] == EDGE){ move.Set(x - 1, y, player); }
				if (board[x][y + 1] == EDGE){ move.Set(x, y + 1, player); }
				if (board[x][y - 1] == EDGE){ move.Set(x, y - 1, player); }
				GameMove(move, show_msg);	//capture the last edge of a dead box. 
				return true;
			}
		}
	}
	return false;	//return false if there is no dead box
}
void ChessBoard::CaptureAllDeadBox(sint player, bool show_msg)
{
	//capture all dead box in this chessboard.
	//repetitive execution untile no dead box exists;
	for (; CaptureDeadBox(player,show_msg););
}
sint ChessBoard::RandomMoveWithBias(sint player, bool show_msg)
{
	CaptureAllDeadBox(player, show_msg);//capture all dead boxes at first(if there is)
	Move moves[MOVENUM];
	int move_num = GetMovesWithBias(moves, player);
	if (move_num == 0)//if there is not a reasonable move after we capture all dead box
	{
		return player;//the current player.
	}
	else
	{
		int random = rand() % move_num;	//get a random number below move_num
		GameMove(moves[random], show_msg);//excute random move
		return -player;//return another player.
	}
	return -player;
}



//some function

void Cprintf(char* str, WORD color, ...) 
{
	WORD colorOld;
	HANDLE handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(handle, &csbi);
	colorOld = csbi.wAttributes;
	SetConsoleTextAttribute(handle, color);
	std::cout << str;
	SetConsoleTextAttribute(handle, colorOld);
	/*
	cprintf("H", 10);	//Green
	cprintf("e", 9);	//Blue
	cprintf("l", 12);	//Red
	cprintf("l", 11);	//Cyan
	cprintf("o", 13);	//Pink
	cprintf("W", 15);	//White
	cprintf("o", 2);	//Deep Green
	cprintf("r", 5);	//Violet
	cprintf("l", 8);	//Grey
	cprintf("d", 14);	//Yellow
	cprintf("!", 4);	//Brown
	*/
}
void CprintfNum(int num, int color)
{
	char str[4];
	sprintf_s(str, "%d", num);
	Cprintf(str, color);
}

bool EqualLoc(Loc &a, Loc &b)
{
	return ((a.x == b.x) && (a.y == b.y));
}
bool EqualChessBoardArray(ChessBoardArray &a, ChessBoardArray &b)
{
	for (int y = 0; y < LEN; y++)
		for (int x = 0; x < LEN; x++)
			if (a[x][y] != b[x][y])
				return false;
	return true;
}
Loc NewLoc(sint x, sint y)
{
	Loc l(x, y);
	return l;
}
Loc NewLoc(int x, int y)
{
	sint lx = (sint)x;
	sint ly = (sint)y;
	Loc l(lx, ly);
	return l;
}
Move NewMove(sint x, sint y, sint p)
{
	Move m(x, y, p);
	return m;
}
Move NewMove(int x, int y, int p)
{
	sint sx = x;
	sint sy = y;
	sint sp = p;
	Move m(sx, sy, sp);
	return m;
}
bool OddNum(sint num)
{
	//return 'true' if the num is an odd number.
	if (num % 2 != 0)
		return true;
	return false;
}
bool OddNum(int num)
{
	//return 'true' if the num is an odd number.
	if (num % 2 != 0)
		return true;
	return false;
}
bool EvenNum(sint num)
{
	//return 'true' if the num is an even number or zero.
	//notice: we define zero as an even number.
	if (num % 2 == 0 || num == 0)
		return true;
	return false;
}
bool EvenNum(int num)
{
	//return 'true' if the num is an even number or zero.
	//notice: we define zero as an even number.
	if (num % 2 == 0 || num == 0)
		return true;
	return false;
}