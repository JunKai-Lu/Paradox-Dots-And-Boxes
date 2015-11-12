#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include <math.h>

#include "GameDefine.h"
#include "StateAnalysis.h"
#include "SearchAlgorithm.h"

using namespace std;

MctsMove::~MctsMove()//the destructor function need to delete the corresponded node if it is not nullptr;
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
	MctsMove *move = &move_list_head;
	//MctsMove *
	for (sint y = 0; y < LEN; y=y+2)
	{
		for (sint x = 1; x < LEN; x=x+2)
		{
			//in each iterator we check (x,y) and (y,x).
			if (board[x][y] == EDGE)
			{
				//we assume there is no any dead box need to be capture.
				//NOTICE: x is always odd number and x is allways even num, so the edge (x,y) is always a horizontal edge.
				//thus we only need check box(x,y+1) and box(x,y-1).
				
				board[x][y] = owner;//assume this player capture the edge.

				if (
					(y == 0 && GetBoxBelongToDeadChainBool(x, y + 1)) ||
					(y == LEN - 1 && GetBoxBelongToDeadChainBool(x, y - 1)) ||
					(y>0 && y < LEN - 1 && GetBoxBelongToDeadChainBool(x, y + 1) && GetBoxBelongToDeadChainBool(x, y - 1))
					)
				{
					//if the conditions are met, the edge is an available move.
					//save the move.

					move->Set(x, y, owner, new MctsMove, nullptr);	//save the move to move linked list.
					move = move->next_move;							//next empty move.
					move_num++;										//increase the number of move
				}

				board[x][y] = EDGE;//recovery the chess board.
			}

			
			if (board[y][x] == EDGE)
			{
				//we assume there is no any dead box need to be capture.
				//NOTICE: x is always odd number and y is allways even num, so the edge (y,x) is always a vertical edge.
				//thus we only need check box( y+1,x ) and box(y-1,x).

				board[x][y] = owner;//assume this player capture the edge.

				if (
					(y == 0 && GetBoxBelongToDeadChainBool(y + 1,x)) ||
					(y == LEN - 1 && GetBoxBelongToDeadChainBool(y - 1,x)) ||
					(y>0 && y < LEN - 1 && GetBoxBelongToDeadChainBool(y + 1,x) && GetBoxBelongToDeadChainBool(y - 1,x))
					)
				{
					//if the conditions are met, the edge is an available move.
					//save the move.

					move->Set(x, y, owner, new MctsMove, nullptr);	//save the move to move linked list.
					move = move->next_move;							//next empty move.
					move_num++;										//increase the number of move
				}
				board[x][y] = EDGE;		//recovery the chess board.
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