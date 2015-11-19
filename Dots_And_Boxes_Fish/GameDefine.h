#pragma once

#include "stdafx.h"
#include "windows.h"
#include <iostream>




//BASIC DEFINITION


//LEN is the length of array.
//BOXLEN is the length of the game grid. in this program only square grid can be defined.
//BOXNUM is the total number of boxes.
//MoveNUM is the total number of moves.
enum GameConstant
{
	EMPTY = 0, LEN = 11, BOXLEN = 5, BOXNUM = 25, MOVENUM = 60
};

//here we define some constant express the different piece in the array
enum ChessBoardType
{
	DOT = 0, RED = 1, BLUE = -1, RED_BOX = 2, BLUE_BOX = -2, EDGE = 10, BOX = 20
};

//we define the types of box as follow:
enum BoxType
{
	FULL_BOX = 0, DEAD_BOX = 1, CHAIN_BOX = 2, FREE_BOX = 3
};

//'char' is redefined as 'sint' for easier substitution. 
//'char' instead of 'int' in order to save memory.

typedef short sint;
using ChessBoardArray = sint[LEN][LEN];


//debug model
#define DEBUG true



//CLASS DEFINITION

//class Loc mean LocATION. which consist of x and y.
class Loc
{
friend bool EqualLoc(Loc &a, Loc &b);
protected:
	sint x;
	sint y;
public:
	Loc()
	{
		x = 0;
		y = 0;
	}
	Loc(sint lx, sint ly)
	{
		x = lx;
		y = ly;
	}
	virtual void Set(sint lx, sint ly)
	{
		x = lx;
		y = ly;
	}
	
};


//class Move is and standard move which include the location and player, and a POINTER to the new board.
class Move :public Loc
{
friend class ChessBoard;

protected:
	sint player = 0;
public:
	Move() :Loc()
	{
		player = 0;
	}
	Move(sint lx, sint ly, sint lp) :Loc(lx, ly)
	{
		player = lp;
	}
	virtual void Set(sint lx, sint ly, sint lp)
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
class 'ChessBoard' is the foundational class, which included an array that express an standard chess board.
it is a base class.
*/
class ChessBoard
{
friend class ChessBoardSolver;
friend class MctsSearch;
friend class MctsNode;
protected:
	//data
	ChessBoardArray board;//this array express an standard chess board.

public:
	ChessBoard();												//constructed function
	ChessBoard(ChessBoardArray &chessboard);					//constructed function
	ChessBoard(ChessBoardArray &chessboard, Move &move);		//constructed function
	void GameMove(Move &Move, bool show_msg);					//the basic function of this game. only through it can u change the chessboard.
	void GameMoveMsg(Move &m);									//sent a move message
	void SetChessBoard(ChessBoardArray &source);				//set the value chess board array through copy from &source.
	sint Winner() const;										//return a winner by normal game rule
	sint ComputeWinner(sint next_player);						//compute the winner of current state by analysis the situation.
	void PrintCB() const;										//print the chessboard

public:
	bool GetBoxBelongToDeadChainBool(sint box_x, sint box_y)const;	//get whether a box belong to a dead chain
	sint GetMovesWithBias(Move moves[MOVENUM], sint player);		//a different function which would save all possible moves to a move array
	bool ExistMoveWithBoas();										//check if any bias move exist.
	bool CaptureDeadBox(sint player, bool show_msg);				//capture a dead box, return false if there is no any dead box.
	void CaptureAllDeadBox(sint player, bool show_msg);				//capture all dead box in this chessboard.
	void RandomMoveWithBias(sint player, bool show_msg);			//take a random move(untile finish)

protected:
	
	inline int GetPlayerBoxes(sint player) const
	{
		int b = 0;
		sint box = player * 2;
		for (int i = 0; i < LEN; i++)
			for (int j = 0; j < LEN; j++)
				if (board[i][j] == box)
					b++;
		return b;
	}
	inline int GetBoxLiberties(sint x, sint y) const
	{
		int edge = 0;
		if (board[x + 1][y] == EDGE){ edge++; }
		if (board[x - 1][y] == EDGE){ edge++; }
		if (board[x][y + 1] == EDGE){ edge++; }
		if (board[x][y - 1] == EDGE){ edge++; }
		return edge;
	}
	inline bool GetBoxCompleted(sint x, sint y) const
	{
		if (board[x + 1][y] == EDGE){ return false; }
		if (board[x - 1][y] == EDGE){ return false; }
		if (board[x][y + 1] == EDGE){ return false; }
		if (board[x][y - 1] == EDGE){ return false; }
		return true;
	}
	inline sint GetBoxOwner(sint x, sint y) const
	{
		if (board[x][y] == RED_BOX){ return RED; }
		if (board[x][y] == BLUE_BOX){ return BLUE; }
		return 0;
	}


};


//basic function

/*
'Cprintf' can print colorful string.
'CprintfNum' can print colorful integer need not convert int to string. 
*/
void Cprintf(char* str, WORD color, ...);
void CprintfNum(int num, int color);

bool EqualLoc(Loc &a, Loc &b);
bool EqualChessBoard(ChessBoardArray &a, ChessBoardArray &b);

Loc NewLoc(sint x, sint y);
Loc NewLoc(int x, int y);
Move NewMove(sint x, sint y, sint p);
Move NewMove(int x, int y, int p);
bool OddNum(sint num);
bool OddNum(int num);
bool EvenNum(sint num);
bool EvenNum(int num);

