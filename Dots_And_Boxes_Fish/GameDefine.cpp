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
ChessBoard::ChessBoard(ChessBoard &chessboard)
{
	SetChessBoard(chessboard.board);
}
ChessBoard::ChessBoard(ChessBoard &chessboard, Move &move)
{
	//copy the chessboard first then make one move in order to create a new state.
	SetChessBoard(chessboard.board);
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
sint ChessBoard::Winner()
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
void ChessBoard::PrintCB()
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