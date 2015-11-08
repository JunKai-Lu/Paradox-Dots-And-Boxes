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
void BOARD::Move(MOVE &Move, bool &ShowMsg)
{
	//if 'ShowMsg' is true then sent message.
	if (ShowMsg)
		MoveMsg(Move);

	//change the value of the location of board.
	board[Move.x][Move.y] = Move.player;
	
	//check if there is any box is completed.

	if (OddNum(Move.x)&&EvenNum(Move.y))//horizonal
	{
		
	}
	else if (OddNum(Move.y) && EvenNum(Move.x))//vertical
	{

	}
	else
	{
		cout << "WARNING: [Wrong Move]" << endl;
		system("pause");
	}
	

}
void BOARD::SetBoard(Board &Source)
{
	for (int i = 0; i < LEN; i++)
		for (int j = 0; j < LEN; j++)
			board[i][j] = Source[i][j];
}
inline void BOARD::MoveMsg(MOVE &m)
{
	cout << "===== Move Message =====" << endl;
	cout << "Msg: ";
	if (m.player == RED)
		Cprintf("Red", 12);
	else
		Cprintf("Blue ", 9);
	cout << " Captual Edge";
	Cprintf("(", 10);
	CprintfNum(m.x,10);
	Cprintf(",", 10);
	CprintfNum(m.y, 10);
	Cprintf(")\n", 10);
	cout << "===== Move Message =====" << endl;
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
LOC NewLoc(sint &x, sint &y)
{
	LOC l(x, y);
	return l;
}
MOVE NewMove(sint &x, sint &y, sint &p)
{
	MOVE m(x, y, p);
	return m;
}
bool EqualBoard(Board &a, Board &b)
{
	for (int y = 0; y < LEN; y++)
		for (int x = 0; x < LEN; x++)
			if (a[x][y] != b[x][y])
				return false;
	return true;
}
bool OddNum(sint &num)
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