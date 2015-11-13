#include "stdafx.h"
#include "windows.h"
#include <iostream>

#include "GameDefine.h"
#include "StateAnalysis.h"


using namespace std;



sint ChessBoard::GetStateWinner(sint NextPlayer)
{
	BoxesArray boxes_type;
	DefineBoxesType(boxes_type);//Define all chain box

			

	return 0;
}

bool ChessBoard::GetBoxBelongToDeadChainBool(sint box_x, sint box_y)
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
bool ChessBoard::AnyBoxBelongToDeadChain()
{
	for (int box_y = 1; box_y < LEN; box_y=box_y+2)
	{
		for (int box_x = 1; box_x < LEN; box_x=box_x+2)
		{
			if (GetBoxLiberties(box_x, box_y) == DEAD_BOX)//check if the box is a dead box
			{
				int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				for (int n = 0; n < 4; n++)
				{
					int edge_x = box_x + Dir[n][0];	//the location of one of its edge.
					int edge_y = box_y + Dir[n][1];
					int next_box_x = box_x + Dir[n][0] + Dir[n][0];//the location of next box
					int next_box_y = box_y + Dir[n][1] + Dir[n][1];
					if (board[edge_x][edge_y] == EDGE&&next_box_x >= 1 && next_box_x <= LEN - 2 && next_box_y >= 1 && next_box_y <= LEN - 2)
					{
						if (GetBoxLiberties(next_box_x, next_box_y) == CHAIN_BOX)
							return true;
					}
				}
			}
		}
	}
	return false;
}