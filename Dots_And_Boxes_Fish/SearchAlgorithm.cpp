#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include <math.h>

#include "GameDefine.h"
#include "StateAnalysis.h"
#include "SearchAlgorithm.h"

using namespace std;

MctsMove::~MctsMove()
{
	// if those pointer are not nullptr , the object they pointed to should be deleted.
	if (corresponded_node != nullptr)
		delete corresponded_node;
	if (next_move != nullptr)
		delete next_move;
}

sint MctsNode::GetMovesWithBias()
{
	int move_num = 0;
	//MctsMove *
	for (int y = 0; y < LEN; y=y+2)
	{
		for (int x = 1; x < LEN; x=x+2)
		{
			//in each iterator we check (x,y) and (y,x).
			if (board[x][y] == EDGE)
			{

			}

			if (board[y][x] == EDGE)
			{

			}
		}
	}
	return move_num;
}