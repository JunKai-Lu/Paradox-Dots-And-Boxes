#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include <math.h>

#include "GameDefine.h"
#include "StateAnalysis.h"
#include "SearchAlgorithm.h"

using namespace std;

MctsNode::MctsNode() :ChessBoard()
{
	//this is an empty object
	owner = RED;				//assume the owner is RED for all empty object
	node_winner = Winner();
	move_list_head.Set(0, 0, 0, nullptr, nullptr);
	avg_value = 0;
	visited_times = 0;			//an empty object have never been visited.
	total_child_node_num = 0;
	existed_child_node_num = 0;
}
MctsNode::MctsNode(ChessBoard &cb, sint node_owner) :ChessBoard(cb.board)
{
	owner = node_owner;							//initilize the 'owner' equal to the 'node_owner'
	node_winner = Winner();						//that should change to GetStateWinner() but the func is still uncomplete.
	visited_times = 1;							//initilize 'visited_times' to 1 because create node equal to first visit.
	total_child_node_num = GetMovesWithBias();	//compute all available moves and save them as a linked list.
	existed_child_node_num = 0;					//do not have any child node.
	avg_value = 0;								//initilize to 0
	//
}
MctsNode::MctsNode(ChessBoard &cb, Move &next_move, sint node_owner) :ChessBoard(cb.board, next_move)
{
	owner = node_owner;							//initilize the 'owner' equal to the 'node_owner'
	node_winner = Winner();						//that should change to GetStateWinner() but the func is still uncomplete.
	visited_times = 1;							//initilize 'visited_times' to 1 because create node equal to first visit.
	total_child_node_num = GetMovesWithBias();	//compute all available moves and save them as a linked list.
	existed_child_node_num = 0;					//do not have any child node.
	avg_value = 0;								//initilize to 0
	//
}
MctsMove::~MctsMove()//the destructor function need to delete the corresponded node if it is not nullptr;
{
	// if those pointer are not nullptr , the object they pointed to should be deleted.
	if (corresponded_node != nullptr)
		delete corresponded_node;
	if (next_move != nullptr)
		delete next_move;
}

//this two function is used to judge if any dead chain exist

bool MctsNode::AnyBoxBelongToDeadChain()
{
	for (int box_y = 1; box_y < LEN; box_y = box_y + 2)
	{
		for (int box_x = 1; box_x < LEN; box_x = box_x + 2)
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

//this two function is used to get all possible moves

sint MctsNode::GetMovesWithBias()
{
	int move_num = 0;
	MctsMove *move = &move_list_head;
	//MctsMove *
	for (sint y = 0; y < LEN; y = y + 2)
	{
		for (sint x = 1; x < LEN; x = x + 2)
		{
			//in each iterator we check (x,y) and (y,x).
			if (board[x][y] == EDGE)
			{
				//we assume there is no any dead box need to be capture.
				//NOTICE: x is always odd number and x is allways even num, so the edge (x,y) is always a horizontal edge.
				//thus we only need check box(x,y+1) and box(x,y-1).
				if (
					(y == 0 && !EdgeCauseDeadChain(x, y, x, y + 1)) ||
					(y == LEN - 1 && !EdgeCauseDeadChain(x, y, x, y - 1)) ||
					(y>0 && y < LEN - 1 && !EdgeCauseDeadChain(x, y, x, y + 1) && !EdgeCauseDeadChain(x, y, x, y - 1))
					)
				{
					//if the conditions are met, the edge is an available move.
					//save the move.

					move->Set(x, y, owner, new MctsMove, nullptr);	//save the move to move linked list.
					move = move->next_move;							//next empty move.
					move_num++;										//increase the number of move
				}
			}


			if (board[y][x] == EDGE)
			{
				//we assume there is no any dead box need to be capture.
				//NOTICE: x is always odd number and y is allways even num, so the edge (y,x) is always a vertical edge.
				//thus we only need check box( y+1,x ) and box(y-1,x).
				if (
					(y == 0 && !EdgeCauseDeadChain(y, x, y + 1, x)) ||
					(y == LEN - 1 && !EdgeCauseDeadChain(y, x, y - 1, x)) ||
					(y>0 && y < LEN - 1 && !EdgeCauseDeadChain(y, x, y + 1, x) && !EdgeCauseDeadChain(y, x, y - 1, x))
					)
				{
					//if the conditions are met, the edge is an available move.
					//save the move.

					move->Set(y, x, owner, new MctsMove, nullptr);	//save the move to move linked list.
					move = move->next_move;							//next empty move.
					move_num++;										//increase the number of move
				}
			}
		}
	}
	return move_num;
}




//class 'MctsSearch' is the major searching algorithm in this engine.
//when the object is created, a new root node would be created according to the chessboard.

//public interface.
MctsSearch::MctsSearch(ChessBoard &cb, sint p)
{
	chessboard = &cb;					//the source chessboard.
	root_node = new MctsNode(cb, p);	//create the root node according to the source chessboard.
}

//private function
sint MctsNode::SingleSimulation()
{
	ChessBoard chessboard = *this;	//copy chessboard
	sint player = owner;
	sint winner = chessboard.ComputeWinner(player);
	for (; winner == 0;)
	{
		player = chessboard.RandomMoveWithBias(player, false);
		winner = chessboard.ComputeWinner(player);
	}
	return winner;
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