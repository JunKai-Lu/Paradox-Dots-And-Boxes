#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include <math.h>

#include "GameDefine.h"
#include "StateAnalysis.h"
#include "SearchAlgorithm.h"

using namespace std;

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
				//we assume there is no any dead box need to be capture.
				//NOTICE: x is always odd number and x is allways even num, so the edge (x,y) is always a horizontal edge.
				//thus we only need check box(x,y+1) and box(x,y-1).
				
			}

			
			if (board[y][x] == EDGE)
			{
				//we assume there is no any dead box need to be capture.
				//NOTICE: x is always odd number and y is allways even num, so the edge (y,x) is always a vertical edge.
				//thus we only need check box( y+1,x ) and box(y-1,x).
			}
		}
	}
	return move_num;
}






//in dots-and-boxes, we only conside some reasonable moves but not all moves.
//when we begin our search we do not need conside how many edges should we capture this turn. 
//if we need conside sacrifice or not, it should be treated as a special case.
//if we do not need conside sacrifice, we only need capture all dead boxes then conside next move.
//the pseudo code as follow:
/*

if (dead chain exist)
{
	conside sacrifice or not. as the different child of the root node.
	only have two child would be added to the root node.
	starting with the ordinary MCTS iteration from root node.
	NOTICE: both the two node's initial value is 0.
}
else
{
	capture all dead box.
	conside move from some reasonable moves but not all moves.(normal searching)
}

*/