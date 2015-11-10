#include "stdafx.h"
#include "GameDefine.h"
#include "windows.h"
#include <iostream>

using namespace std;


BOARD::BOARD()
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
BOARD::BOARD(Board &CB)
{
	SetBoard(CB);
}
BOARD::BOARD(Board &CB, MOVE &Move)
{
	SetBoard(CB);
}
void BOARD::Move(MOVE &Move, bool ShowMsg)
{
	/*
	CAUTION: if edge (x,y) and the boxes near by it have been captured, the function will change the boxes' belonger.  
	*/
	if (DEBUG)
	{
		if (board[Move.x][Move.y] != EDGE)
		{
			cout << "WARNING [Wrong Edge Location]" << endl;
			system("pause");
		}
	}

	//if 'ShowMsg' is true then sent message.
	if (ShowMsg)
		MoveMsg(Move);

	//change the value of the location of board.
	board[Move.x][Move.y] = Move.player;
	
	//check if there is any box is completed.

	if (OddNum(Move.x)&&EvenNum(Move.y))//horizonal
	{
		if (Move.y + 1 < LEN - 1)
			if (GetBoxCompleted(Move.x, Move.y + 1))
				board[Move.x][Move.y + 1] = Move.player*2;
		if (Move.y - 1 > 0)
			if (GetBoxCompleted(Move.x, Move.y - 1))
				board[Move.x][Move.y - 1] = Move.player*2;	
	}
	else if (OddNum(Move.y) && EvenNum(Move.x))//vertical
	{
		if (Move.x + 1 < LEN - 1)
			if (GetBoxCompleted(Move.x + 1, Move.y))
				board[Move.x + 1][Move.y] = Move.player*2;
		if (Move.x - 1 > 0)
			if (GetBoxCompleted(Move.x - 1, Move.y))
				board[Move.x - 1][Move.y] = Move.player*2;
	}
	else
	{
		cout << "WARNING: [Wrong Move]" << endl;
		system("pause");
	}
	

}
void BOARD::MoveMsg(MOVE &m)
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
void BOARD::SetBoard(Board &Source)
{
	for (int i = 0; i < LEN; i++)
		for (int j = 0; j < LEN; j++)
			board[i][j] = Source[i][j];
}

sint BOARD::Winner()
{
	int RedBoxes = GetPlayerBoxes(RED);
	int BlueBoxes = GetPlayerBoxes(BLUE);
	if ((RedBoxes + BlueBoxes) >= BOXNUM)
	{
		if (RedBoxes>BlueBoxes)
			return RED;
		else
			return BLUE;
	}
	return 0;
}

void BOARD::PrintBoard()
{
	//Print Chess Board
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
			else if (board[i][j] == RED_EDGE)
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
			else if (board[i][j] == BLUE_EDGE)
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
void CprintfNum(int Num, int color)
{
	char str[4];
	sprintf_s(str, "%d", Num);
	Cprintf(str, color);
}

bool EqualLoc(LOC &a, LOC &b)
{
	return ((a.x == b.x) && (a.y == b.y));
}
bool EqualBoard(Board &a, Board &b)
{
	for (int y = 0; y < LEN; y++)
		for (int x = 0; x < LEN; x++)
			if (a[x][y] != b[x][y])
				return false;
	return true;
}
LOC NewLoc(sint &x, sint &y)
{
	LOC l(x, y);
	return l;
}
LOC NewLoc(int &x, int &y)
{
	sint lx = (sint)x;
	sint ly = (sint)y;
	LOC l(lx, ly);
	return l;
}
MOVE NewMove(sint &x, sint &y, sint &p)
{
	MOVE m(x, y, p);
	return m;
}
MOVE NewMove(int &x, int &y, int &p)
{
	sint sx = x;
	sint sy = y;
	sint sp = p;
	MOVE m(sx, sy, sp);
	return m;
}
bool OddNum(sint &num)
{
	//return 'true' if the num is an odd number.
	if (num % 2 != 0)
		return true;
	return false;
}
bool OddNum(int &num)
{
	//return 'true' if the num is an odd number.
	if (num % 2 != 0)
		return true;
	return false;
}
bool EvenNum(sint &num)
{
	//return 'true' if the num is an even number or zero.
	//notice: we define zero as an even number.
	if (num % 2 == 0 || num == 0)
		return true;
	return false;
}
bool EvenNum(int &num)
{
	//return 'true' if the num is an even number or zero.
	//notice: we define zero as an even number.
	if (num % 2 == 0 || num == 0)
		return true;
	return false;
}