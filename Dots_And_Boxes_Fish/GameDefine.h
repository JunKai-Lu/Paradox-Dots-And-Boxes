#pragma once

#include "stdafx.h"
#include "windows.h"
#include <iostream>




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
here we define some constant express the different piece in the array
*/
#define EMPTY 0
#define RED 1
#define BLUE -1
#define DOT 0
#define EDGE 10
#define BOX 20
#define RED_EDGE 1
#define BLUE_EDGE -1
#define RED_BOX 2
#define BLUE_BOX -2

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
friend bool EqualLoc(LOC &a, LOC &b);
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
	
};

/*
class MOVE is and standard move which include the location and player, and a POINTER to the new board.
*/
class MOVE :public LOC
{
	friend class BOARD;
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
	void Show()
	{
		std::cout << "X=" << (int)x << " Y=" << (int)y << " Player=" << (int)player << std::endl;
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
	void Move(MOVE &Move, bool &ShowMsg);
	inline void MoveMsg(MOVE &m);
	void SetBoard(Board &Source);

private:
	inline bool GetBoxCompleted(sint &x, sint &y)
	{
		if (board[x + 1][y] == EDGE){ return false; }
		if (board[x - 1][y] == EDGE){ return false; }
		if (board[x][y + 1] == EDGE){ return false; }
		if (board[x][y - 1] == EDGE){ return false; }
		return true;
	}
};


//basic function

/*
'Cprintf' can print colorful string.
'CprintfNum' can print colorful integer need not convert int to string. 
*/
void Cprintf(char* str, WORD color, ...);
void CprintfNum(int Num, int color);

bool EqualLoc(LOC &a, LOC &b);
LOC NewLoc(sint &x, sint &y);
MOVE NewMove(sint &x, sint &y, sint &p);
bool EqualBoard(Board &a, Board &b);
bool OddNum(sint &num);
bool EvenNum(sint &num);

