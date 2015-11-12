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
	
	MctsNode *corresponded_node;	//point to the new node which stands the current node implemented this move; 
	MctsMove *next_move;			//point to the next move of current node;

	MctsMove(sint lx, sint ly, sint lp) :Move(lx,ly,lp)
	{
		corresponded_node = nullptr;
		next_move = nullptr;
	}
	virtual void Set(sint &lx, sint &ly, sint &lp,MctsNode *node,MctsMove *move)
	{
		x = lx;
		y = ly;
		player = lp;
		corresponded_node = node;
		next_move = move;
	}
	~MctsMove()//the destructor function need to delete the corresponded node if it is not nullptr;
	{
		// if those pointer are not nullptr , the object they pointed to should be deleted.
		if (corresponded_node != nullptr)
			delete corresponded_node;
		if (next_move != nullptr)
			delete next_move;
	}
	
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
	MctsNode() :ChessBoard()
	{
		
	}
	


	sint owner;						//the player who is preparing to take next move. we define he is the owner of this node.
	sint node_winner;				//if the game do not finish , the value is 0.
	MctsMove *move_list_head;		//this pointer will point to the head of move linked list. 
	float avg_value;				//the avg value of this value, it is equal to [1 - (the average value of all child nodes)].

	//destructor function
	~MctsNode()
	{
		// if the pointer are not nullptr , the 'Move' object it pointed to should be deleted.
		if (move_list_head != nullptr)
			delete move_list_head;
	}

private:
	int visited_times;				//the number of this node has been visited.
	sint total_child_node_num;		//the number of all feasible child node ,whatever it have been created or not.
	sint existed_child_node_num;	//the number of thoes child node have been created)
	
	//class function
	
	sint GetMovesWithBias();		//computing all possible moves with searching bias

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
	inline MctsNode *AddingNode()
	{
		
	}

	//get a 'Move' from moves linked list according the number.
	inline Move *get_next_move()
	{
		MctsMove *node;
		for (node = move_list_head;node->next_move!=nullptr&&node->corresponded_node!=nullptr;node = node->next_move);
		return node;
	}


};

