#pragma once

#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include "GameDefine.h"

enum ChainType
{
	NOT_DEFINED, SINGLE_CHAIN, SHORT_CHAIN, LONG_CHAIN, PRE_LONG_CHAIN, CIRCLE, PRE_CIRCLE, DEAD_CHAIN, DEAD_CIRCLE
};

enum BoxType
{
	FULL_BOX = 0, DEAD_BOX = 1, CHAIN_BOX = 2, FREE_BOX = 3
};

class BoxInfo
{
public:
	BoxType type;				//include CPUTURED, DEAD_BOX, CHAIN_BOX, FREE_BOX
	int belonging_chain_num;	//the number of the chain which this box belong.
};

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
	ChessBoardSolver(ChessBoard &chessboard)
	{
		sample = &chessboard;
	}

private:
	ChessBoard *sample;				//analytical sample
	BoxInfo boxes[BOXLEN][BOXLEN];	//boxes infomation
	ChainInfo chains[BOXNUM];		//chains infomation

	sint first_player;				//the player who is preparing to make move.
	void RefreshBoxesInfo();		//refresh the infomation of all boxes 
};