#include "stdafx.h"
#include "windows.h"
#include <iostream>

#pragma  once
//BASIC DEFINITION

/*
LEN is the length of array.
BOXLEN is the length of the game grid. in this program only square grid can be defined.
BOXNUM is the total number of boxes.
MOVENUM is the total number of moves.
*/

#define LEN 11
#define BOXLEN ((LEN-1)/2)
#define BOXNUM BOXLEN*BOXLEN
#define MOVENUM BOXLEN*(BOXLEN+1)*2


/* 
'short' is redefined as 'sint' for easier substitution. 
'short' instead of 'int' in order to save memory.
*/
typedef char sint;
using Board = sint[LEN][LEN];



//CLASS DEFINITION

/*
class LOC mean LOCATION. which consist of x and y.
*/
class LOC
{
protected:
	sint x = 0;
	sint y = 0;
public:
	LOC::LOC(sint &lx, sint &ly)
	{
		x = lx;
		y = ly;
	}
	virtual void Set(sint &lx, sint &ly)
	{
		x = lx;
		y = ly;
	}
	friend bool EqualLoc(LOC &a, LOC &b);
};

/*
class MOVE is and standard move which include the location and player, and a POINTER to the new board.
*/
class MOVE :public LOC
{
protected:
	sint player = 0;
public:
	MOVE(sint &lx, sint &ly, sint &lp) :LOC(lx, ly)
	{
		player = lp;
	}
	void Set(sint &lx, sint &ly, sint &lp)
	{
		x = lx;
		y = ly;
		player = lp;
	}
};


/*
class 'BOARD' is the foundational class, which included an array that express an standard chess board.
it is a base class.
*/
class BOARD
{
public:
	//data
	Board board;//this array express an standard chess board.

	//function
	BOARD();
	BOARD(Board &CB);
	BOARD(Board &CB, MOVE &Move);

};


//basic function

/*
'Cprintf' can print colorful string.
'CprintfNum' can print colorful integer need not convert int to string. 
*/
void Cprintf(char* str, WORD color, ...) {
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
void CprintfNum(int Num,int color)
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
	LOC l(x,y);
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
bool OddNum(int &num)
{
	//return 'true' if the num is an odd number.
	if (num % 2 != 0)
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