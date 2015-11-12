#pragma once

#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include "GameDefine.h"

enum ChainType
{
	NOT_DEFINE, SINGLE_CHAIN, SHORT_CHAIN, LONG_CHAIN, PRE_LONG_CHAIN, CIRCLE, PRE_CIRCLE, DEAD_CHAIN, DEAD_CIRCLE
};

enum BoxType
{
	CPUTURED, DEAD_BOX, CHAIN_BOX, FREE_BOX
};

class BoxInfo
{
public:
	BoxType Type;				//include CPUTURED, DEAD_BOX, CHAIN_BOX, FREE_BOX
	int BelongingChainNum;		//the number of the chain which this box belongs.
};

class ChainInfo
{
public:
	ChainInfo()
	{
		chain_type = NOT_DEFINE;
		total_box_num = 0;
	}
	ChainType chain_type;
	int total_box_num;
	Loc start_loc;
	Loc end_loc;
};

bool ChessBoard::GetDeadChainExist(sint box_x, sint box_y)
{
	if (GetBoxLiberties(box_x, box_y) == DEAD_BOX)//首先这个格子必须本身是一个C型格
	{
		int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
		for (int n = 0; n < 4; n++)
		{
			int edge_x = box_x + Dir[n][0];
			int edge_y = box_y + Dir[n][1];
			int next_box_x = box_x + Dir[n][0] + Dir[n][0];//下一个格子的实际地址
			int next_box_y = box_y + Dir[n][1] + Dir[n][1];
			if (board[edge_x][edge_y] == EDGE&&next_box_x >= 1 && next_box_x <= LEN - 2 && next_box_y >= 1 && next_box_y <= LEN - 2)
			{
				if (GetBoxLiberties(next_box_x, next_box_y) == CHAIN_BOX)
					return true;
			}
		}
	}
	return false;
}