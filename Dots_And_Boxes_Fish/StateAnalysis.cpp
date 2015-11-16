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

void ChessBoardSolver::RefreshBoxesInfo()
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

//test function
void ChessBoardSolver::ShowBoxType()
{
	RefreshBoxesInfo();
	for (int j = 0; j < BOXLEN; j++)
	{
		for (int i = 1; i < BOXLEN; i++)
		{
			if (boxes[i][j].type == FULL_BOX)
				Cprintf("¡ö", 8);
			else if (boxes[i][j].type == DEAD_BOX)
				Cprintf("¡ö", 2);
			else if (boxes[i][j].type == CHAIN_BOX)
				Cprintf("¡ö", 14);
			else if (boxes[i][j].type == FREE_BOX)
				Cprintf("¡ö", 5);
		}
		cout << endl;
	}
	Cprintf("\n¡ö [Free Box]", 5);
	Cprintf("\n¡ö [Chain Box]", 14);
	Cprintf("\n¡ö [Dead Box]", 2);
	Cprintf("\n¡ö [Full Box]\n", 8);
}

