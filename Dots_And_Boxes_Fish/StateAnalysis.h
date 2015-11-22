#pragma once

#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include "GameDefine.h"

//we define the types of chain as follow:
enum ChainType
{
	UNDEFINED, CHAIN, OPEN_CHAIN, CIRCLE, OPEN_CIRCLE, DEAD_CHAIN, DEAD_CIRCLE
};



//the infomation of a box in chessboard.
class BoxInfo
{
public:
	BoxInfo()
	{
		belonging_chain_num = 0;
	}
	BoxType type;				//include FULL_BOX, DEAD_BOX, CHAIN_BOX, FREE_BOX
	int belonging_chain_num;	//the number of the chain which this box belong.
};

//the infomation of a chain in chessboard.
class ChainInfo
{
public:
	ChainInfo()
	{
		chain_type = UNDEFINED;
		total_box_num = 0;
	}
	ChainType chain_type;
	int total_box_num;
	Loc start_loc;
	Loc end_loc;
};


class ChessBoardSolver
{
public:
	ChessBoardSolver(ChessBoard &cb, sint next_player);
	sint CalculateWinner();			//return the winner of this state

public:
	//data member
	ChessBoard *chessboard;			//analytical sample
	BoxInfo boxes[BOXLEN][BOXLEN];	//boxes infomation
	ChainInfo chains[BOXNUM];		//chains infomation
	sint first_player;				//the player who is preparing to make move.

	//function
	void DefineBoxesInfo();									//define the infomation of all boxes 
	void DefineChainInfo();									//define the infomation of all chains
	Loc GetNextBox(Loc dest, Loc source);					//find another adjacent box of dest box but the source box.
	void SearchingFromFreeBox(Loc free_box_loc);			//define chain from a undefined free box.
	void SearchingCircle(Loc chain_box_loc);				//define circle form a undefined chain box. 
	void RegisterChain(Loc free_box_loc, Loc first_loc);
	void RegisterCircle(Loc start_loc, Loc first_loc);

	int GetEmptyChainNum();									//return a number of an empty chain
	BoxType GetBoxType(sint bx, sint by);					//return the type of box (bx,by). all boxes which is out of chessboard would be judged as FREE_BOX.		
	void InheritChain(int inheritor, int ancester);			//one chain get all the box of another chain

	//test
	void ShowBoardInfo() const;
};