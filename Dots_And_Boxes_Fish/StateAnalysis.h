#pragma once

#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include "GameDefine.h"

//we define the types of chain as follow:
enum ChainType
{
	NOT_DEFINED, SINGLE_CHAIN, SHORT_CHAIN, LONG_CHAIN, PRE_LONG_CHAIN, CIRCLE, PRE_CIRCLE, DEAD_CHAIN, DEAD_CIRCLE
};

//we define the types of box as follow:
enum BoxType
{
	FULL_BOX = 0, DEAD_BOX = 1, CHAIN_BOX = 2, FREE_BOX = 3
};

//the infomation of a box in chessboard.
class BoxInfo
{
public:
	BoxType type;				//include FULL_BOX, DEAD_BOX, CHAIN_BOX, FREE_BOX
	int belonging_chain_num;	//the number of the chain which this box belong.
};

//the infomation of a chain in chessboard.
class ChainInfo
{
public:
	ChainInfo()
	{
		chain_type = NOT_DEFINED;
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

private:
	//data member
	ChessBoard *chessboard;			//analytical sample
	BoxInfo boxes[BOXLEN][BOXLEN];	//boxes infomation
	ChainInfo chains[BOXNUM];		//chains infomation
	sint first_player;				//the player who is preparing to make move.

	//function
	void RefreshBoxesInfo();		//refresh the infomation of all boxes 

	//test
	void ShowBoxType();
};