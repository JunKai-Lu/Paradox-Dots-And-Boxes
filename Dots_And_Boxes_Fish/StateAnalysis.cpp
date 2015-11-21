#include "stdafx.h"
#include "windows.h"
#include <iostream>

#include "GameDefine.h"
#include "StateAnalysis.h"
#include "SearchAlgorithm.h"


using namespace std;

//public interface
ChessBoardSolver::ChessBoardSolver(ChessBoard &cb, sint next_player)
{
	chessboard = &cb;
	first_player = next_player;
}
sint ChessBoardSolver::CalculateWinner()
{
	
	
	return 0;
}

//private function

void ChessBoardSolver::DefineBoxesInfo()
{
	//x = bx*2+1  y = by*2+1
	//the (x,y) mean the actual adress of the box whose address is (bx,by) in boxes infomation array.
	for (sint by = 0; by < BOXLEN; by++)
	{
		for (sint bx = 0; bx < BOXLEN; bx++)
		{
			sint x = (bx * 2) + 1;					//the actual adress
			sint y = (by * 2) + 1;					//the actual adress
			
			//define the box type
			int bl = chessboard->GetBoxLiberties(x, y);	//obtain the liberties of the boxes
			if (bl >= 3)
				boxes[bx][by].type = FREE_BOX;
			else if (bl == 1)
				boxes[bx][by].type = DEAD_BOX;
			else if (bl == 2)
				boxes[bx][by].type = CHAIN_BOX;
			else 
				boxes[bx][by].type = FREE_BOX;
				
		}
	}
}
void ChessBoardSolver::DefineChainInfo()
{
	for (int by = 0; by < BOXLEN; by++)
	{
		for (int bx = 0; bx < BOXLEN; bx++)
		{
			if (boxes[bx][by].type == FREE_BOX)
			{

			}
		}
	}
}
Loc ChessBoardSolver::GetNextBox(Loc source, Loc dest)
{
	//notice: the returned box location maybe in outside of chessboard. 
	int dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	int dest_x = (dest.x * 2) + 1;
	int dest_y = (dest.y * 2) + 1;
	for (int n = 0; n < 4; n++)
	{
		Loc next_box = Loc(dest.x + dir[n][0], dest.y + dir[n][1]);
		//if the adjacent box is not the source box and the adjacent edge is empty
		if (chessboard->Equal(dest_x + dir[n][0], dest_y + dir[n][1], EDGE) && !EqualLoc(source, next_box))
		{
			return next_box;
		}
	}
	return Loc(EMPTY, EMPTY);
}

int ChessBoardSolver::GetEmptyChainNum()
{
	//return an number of chain which is empty but 0.
	//chain no.0 is always empty.
	for (int i = 1; i < BOXNUM; i++)
		if (chains[i].chain_type == NOT_DEFINED)
			return i;
	return 0;
}
void ChessBoardSolver::InheritChain(int inheritor, int ancester)
{
	//all boxes belong to ancester would be change
	for (int j = 1; j <= BOXLEN; j++)
		for (int i = 1; i <= BOXLEN; i++)
			if (boxes[i][j].belonging_chain_num == ancester)
				boxes[i][j].belonging_chain_num = inheritor;

	chains[inheritor].total_box_num += chains[ancester].total_box_num;

	//reset the data of ancester chain
	chains[ancester].total_box_num = 0;
	chains[ancester].chain_type = NOT_DEFINED;
}


//test function
void ChessBoardSolver::ShowBoxType()
{
	DefineBoxesInfo();
	for (int j = 0; j < BOXLEN; j++)
	{
		for (int i = 1; i < BOXLEN; i++)
		{
			if (boxes[i][j].type == FULL_BOX)
				Cprintf("■", 8);
			else if (boxes[i][j].type == DEAD_BOX)
				Cprintf("■", 2);
			else if (boxes[i][j].type == CHAIN_BOX)
				Cprintf("■", 14);
			else if (boxes[i][j].type == FREE_BOX)
				Cprintf("■", 5);
		}
		cout << endl;
	}
	Cprintf("\n■ [Free Box]", 5);
	Cprintf("\n■ [Chain Box]", 14);
	Cprintf("\n■ [Dead Box]", 2);
	Cprintf("\n■ [Full Box]\n", 8);
}



/*void BoxBoard::SearchingFromBox(LOC BoxLoc)
{
	int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	for (int n = 0; n < 4; n++)
	{
		int bx = BoxLoc.x + Dir[n][0];
		int by = BoxLoc.y + Dir[n][1];
		int lx = (BoxLoc.x * 2) - 1 + Dir[n][0];
		int ly = (BoxLoc.y * 2) - 1 + Dir[n][1];
		if (board[lx][ly] == EDGE&&GetBoxType(bx, by) == CHAINBOX)//邻边为空，目标格子为链格
		{
			if (Boxes[bx][by].BelongingChainNum == EMPTY)//必须为未归属的格子，避免环重复从不同方向出发。
			{
				LOC NewLoc;
				NewLoc.Set(bx, by);
				RegisterChain(BoxLoc, NewLoc);
			}
		}
	}

}*/