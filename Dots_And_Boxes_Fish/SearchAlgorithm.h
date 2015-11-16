#pragma once

#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include "GameDefine.h"

class MctsNode;		//pre declaration.

//class MctsMove is used to express a MOVE and it include a pointer which point to a corresponding node.
//we use linked list to express all the possible move of a node.
//so the 'next_move' point to another move of the current node.
//the 'corresponded_node' point to the the new node which stands the current node implemented this move; 
class MctsMove :public Move
{
public:
	
	//we set 'corresponded_node' and 'next_move' as nullptr
	//once it point to any class ,that we must 'delete' thoes object before current class is released.
	//if the pointer is nullptr that means we do not crearte a corresponding object for this pointer.
	
	MctsMove *next_move;			//point to the next move of current node;
	MctsNode *corresponded_node;	//point to the new node which stands the current node implemented this move; 

	MctsMove() :Move()
	{
		corresponded_node = nullptr;
		next_move = nullptr;
	}
	MctsMove(sint lx, sint ly, sint lp) :Move(lx,ly,lp)
	{
		corresponded_node = nullptr;
		next_move = nullptr;
	}
	virtual void Set(sint lx, sint ly, sint lp, MctsMove *move, MctsNode *node)
	{
		x = lx;
		y = ly;
		player = lp;
		next_move = move;
		corresponded_node = node;
	}
	~MctsMove();
};


//class MctsNode means a node of searching tree.
//any 'MctsNode' object include complete chessboard infomation.
//when 'MctsNode' object is created, all available moves would be defined as a linked list.
//NOTICE: the 'avg_value' is the victory rate of the current node's father node.
class MctsNode:public ChessBoard
{	
	//when constructed function work, it will
	//initilize the 'owner' equal to the 'node_owner' in constructed function argument list (if this argument exist)
	//computing whether there is an winner exists, if true, set 'node_winner' equal to the winner, if false, set the value to 0;
	//computing all the possible moves of this node as a linked list, then set 'total_child_node_num' equal to the number of possible moves.
	//the set 'move_list_head' point to the head of the move linked list.
	//initilize 'existed_child_node_num' to 0 because there is no child exists when this node was just been created.
	//initilize 'visited_times' to 1 because create node equal to first visit.
	//initilize 'avg_value' to 0 because the average value of this node is unknown.

public:
	MctsNode();															//constructed function
	MctsNode(ChessBoard &chessboard, sint node_owner);					//constructed function
	MctsNode(ChessBoard &chessboard, Move &next_move, sint node_owner);	//constructed function

	sint owner;						//the player who is preparing to take next move. we define he is the owner of this node.
	sint node_winner;				//if the game do not finish , the value is 0.
	MctsMove move_list_head;		//this pointer will point to the head of move linked list. 
	float avg_value;				//the avg value of this value, it is equal to [1 - (the average value of all child nodes)].
	
private:
	int visited_times;				//the number of this node has been visited.
	sint total_child_node_num;		//the number of all feasible child node ,whatever it have been created or not.
	sint existed_child_node_num;	//the number of thoes child node have been created)
	
	//class function
	
	

	//compute UCB value for this node when selection.
	inline float get_ucb_value(int total_visit_times)		
	{
		return avg_value + sqrt((((log10((float)total_visit_times)) * 2) / (float)visited_times));
	}	

	//refresh avarage value according the value of child node.
	inline void refresh_avg_value()
	{
		
	}

	//return 'true' if all the child node have been added to the tree.
	inline bool full_child()
	{
		return (total_child_node_num == existed_child_node_num);
	}

	//adding a new node to the searching tree and excuted simulation.
	inline MctsNode *AddingNode(MctsMove next_move)
	{
		
	}

	//get a 'Move' from moves linked list according the number.
	inline Move *get_next_move()
	{
		MctsMove *node;
		for (node = &move_list_head;node->next_move!=nullptr&&node->corresponded_node!=nullptr;node = node->next_move);
		return node;
	}

	/*
	void test_moves()
	{
		total_child_node_num = GetMovesWithBias();
		std::cout << "total moves: [" << (int)total_child_node_num << " ]" << std::endl;
		int i = 0;
		for (MctsMove *move = &move_list_head; move->next_move != nullptr; move = move->next_move)
		{
			std::cout << "["<<i<<"] ";
			i++;
			move->Show();
		}
		std::cout << "all moves have output" << std::endl;
	}
	*/
private:
	sint GetMovesWithBias();									//computing all possible moves with searching bias
	sint GetMovesWithBias(Move moves[MOVENUM], sint player);	//a different function which would save all possible moves to a move array
	bool AnyBoxBelongToDeadChain();
	bool GetBoxBelongToDeadChainBool(sint box_x, sint box_y);	
	
	sint GetStateWinner(sint next_player);
	
	
};

bool CaptureDeadBox(sint player);
void CaptureAllDeadBox(sint player);



//class 'MctsSearch' is the major searching algorithm in this engine.
//when the object is created, a new root node would be created according to the chessboard.


class MctsSearch
{
public:
	MctsSearch(ChessBoard &cb, sint p);		//constructed function
	void MctsMove();						//execute search algorithm

private:
	//data member
	ChessBoard *chessboard;					//source chessboard
	MctsNode *root_node;					//the root node of the search tree.
	sint player;							//the player who is preparing for next move.


	//function
	bool CaptureDeadBox(bool show_msg);		//capture a dead box, return false if there is no any dead box.
	void CaptureAllDeadBox(bool show_msg);	//capture all dead box in this chessboard.
};
