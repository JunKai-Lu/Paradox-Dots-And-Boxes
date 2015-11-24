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
	DefineChainInfo();

	//count the number of each chain.
	int chain_num = 0;
	int open_chain_num = 0;
	int circle_num = 0;
	int open_circle_num = 0;
	int chain_boxes = 0;
	int open_chain_boxes = 0;
	int circle_boxes = 0;
	int open_circle_boxes = 0;
	for (int i = 1; i < BOXNUM; i++)
	{
		if (chains[i].chain_type == CHAIN)
		{
			chain_num++;
			chain_boxes += chains[i].total_box_num;
		}
		else if (chains[i].chain_type == OPEN_CHAIN)
		{
			open_chain_num++;
			open_chain_boxes += chains[i].total_box_num;
		}
		else if (chains[i].chain_type == CIRCLE)
		{
			circle_num++;
			circle_boxes += chains[i].total_box_num;
		}
		else if (chains[i].chain_type == OPEN_CIRCLE)
		{
			open_circle_num++;
			open_circle_boxes += chains[i].total_box_num;
		}


	}
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
				boxes[bx][by].type = FULL_BOX;
				
		}
	}
}
void ChessBoardSolver::DefineChainInfo()
{
	DefineBoxesInfo();

	//define chains from free box whitin board.
	for (int by = 0; by < BOXLEN; by++)
	{
		for (int bx = 0; bx < BOXLEN; bx++)
		{
			if (boxes[bx][by].type == FREE_BOX)
			{
				SearchingFromFreeBox(NewLoc(bx, by));
			}
		}
	}

	//define chains from border of chessboard.
	for (int i = 0; i < BOXLEN; i++)
	{
		SearchingFromFreeBox(NewLoc(-1, i));		//from the left of board 
		SearchingFromFreeBox(NewLoc(BOXLEN, i));	//from the right of board 
		SearchingFromFreeBox(NewLoc(i, -1));		//from the upside of board 
		SearchingFromFreeBox(NewLoc(i, BOXLEN));	//from the downside of board 
	}

	//define circle from undefined chain box.
	for (int i = 0; i <= BOXLEN; i++)
	{
		for (int j = 0; j <= BOXLEN; j++)
		{
			if (boxes[i][j].type == CHAIN_BOX&&boxes[i][j].belonging_chain_num == EMPTY)
			{
				SearchingCircle(NewLoc(i,j));
			}
		}
	}

	ShowBoardInfo();


	//chain plus
	for (int i = 1; i < BOXNUM; i++)
	{
		if (chains[i].chain_type == CHAIN)
		{
			for (int j = i + 1; j <= BOXNUM; j++)
			{
				if (chains[j].chain_type == CHAIN&&EqualLoc(chains[i].start_loc, chains[j].start_loc) && EqualLoc(chains[i].end_loc, chains[j].end_loc))
				{
					if (boxes[chains[j].start_loc.x][chains[j].start_loc.y].belonging_chain_num == EMPTY&&boxes[chains[j].end_loc.x][chains[j].end_loc.y].belonging_chain_num == EMPTY)
					{
						//if two chains have alike start loc and end loc, they are both part of a OPEN_CHAIN.  
						InheritChain(i, j);//chain[i] inherit chain[j];
						boxes[chains[i].start_loc.x][chains[i].start_loc.y].belonging_chain_num = i;
						boxes[chains[i].end_loc.x][chains[i].end_loc.y].belonging_chain_num = i;
						chains[i].total_box_num = chains[i].total_box_num + 2;
						chains[i].start_loc = chains[i].end_loc;
						chains[i].chain_type = OPEN_CHAIN;
					}
				}
			}
		}
	}

	ShowBoardInfo();

	//define OPEN_CHAIN which is derived by OPEN_CIRCLE.
	for (int y = 0; y <= BOXLEN; y++)
	{
		for (int x = 0; x <= BOXLEN; x++)
		{
			if (boxes[x][y].type == FREE_BOX && chains[boxes[x][y].belonging_chain_num].chain_type == OPEN_CIRCLE)
			{
				int dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				int bx = x * 2 + 1;
				int by = y * 2 + 1;
				for (int n = 0; n < 4; n++)
				{
					int ex = bx + dir[n][0];
					int ey = by + dir[n][1];
					int nbx = x + dir[n][0];
					int nby = y + dir[n][1];
					if (chessboard->Equal(ex,ey,EDGE)&&chains[boxes[nbx][nby].belonging_chain_num].chain_type == CHAIN)
					{
						chains[boxes[nbx][nby].belonging_chain_num].chain_type = OPEN_CHAIN;
					}
				}
			}
		}
	}

	ShowBoardInfo();

	//if there is any free box exist, make its two longest CHAIN / OPEN_CHAIN merge then own this box. 
	for (int y = 0; y < BOXLEN; y++)
	{
		for (int x = 0; x <= BOXLEN; x++)
		{
			int dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
			if (boxes[x][y].type == FREE_BOX&&boxes[x][y].belonging_chain_num == EMPTY)//if this box is ownerless.
			{
				int derived_chain_num[4];		//the num of chain which derived by this freebox.
				int total_derived_chain = 0;	//how many derived chains exist.
				for (int n = 0; n < 4; n++)
				{
					int bx = x + dir[n][0];
					int by = y + dir[n][1];
					int ex = (x * 2) + 1 + dir[n][0];
					int ey = (y * 2) + 1 + dir[n][1];
					if (chessboard->Equal(ex, ey, EDGE) && GetBoxType(bx, by) == CHAIN_BOX)//adjacent edge is free and adjacent box is CHAIN_BOX
					{
						derived_chain_num[total_derived_chain] = boxes[bx][by].belonging_chain_num;
						total_derived_chain++;
					}
				}
				//find the longest chain of its dirived chains.
				int first_long_chain = 0;
				int chain_boxes_num = 0;
				for (int i = 0; i < total_derived_chain; i++)
				{
					if (chains[derived_chain_num[i]].total_box_num > chain_boxes_num)
					{
						first_long_chain = derived_chain_num[i];
						chain_boxes_num = chains[derived_chain_num[i]].total_box_num;
					}
				}

				//find the second long chain.
				int second_long_chain = 0;
				chain_boxes_num = 0;
				for (int i = 0; i < total_derived_chain; i++)
				{
					if (chains[derived_chain_num[i]].total_box_num > chain_boxes_num&&derived_chain_num[i] != first_long_chain)
					{
						second_long_chain = derived_chain_num[i];
						chain_boxes_num = chains[derived_chain_num[i]].total_box_num;
					}
				}

				//make the two CHAIN / OPEN_CHAIN merge then own the free box. 
				boxes[x][y].belonging_chain_num = first_long_chain;
				chains[first_long_chain].total_box_num++;
				InheritChain(first_long_chain, second_long_chain);
			}
		}
	}
}
Loc ChessBoardSolver::GetNextBox(Loc dest, Loc source)
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
void ChessBoardSolver::SearchingFromFreeBox(Loc free_box_loc)
{
	int dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	for (int n = 0; n < 4; n++)
	{
		int bx = free_box_loc.x + dir[n][0];
		int by = free_box_loc.y + dir[n][1];
		if (bx >= 0 && bx < BOXLEN&&by >= 0 && by < BOXLEN)
		{
			int lx = (free_box_loc.x * 2) + 1 + dir[n][0];
			int ly = (free_box_loc.y * 2) + 1 + dir[n][1];
			if (chessboard->Equal(lx, ly, EDGE) && GetBoxType(bx, by) == CHAIN_BOX)
			{
				if (boxes[bx][by].belonging_chain_num == EMPTY)
				{
					RegisterChain(free_box_loc, NewLoc(bx, by));
				}
			}
		}
	}

}
void ChessBoardSolver::SearchingCircle(Loc chain_box_loc)
{
	int dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	for (int n = 0; n < 4; n++)
	{
		int bx = chain_box_loc.x + dir[n][0];
		int by = chain_box_loc.y + dir[n][1];
		int lx = (chain_box_loc.x * 2) + 1 + dir[n][0];
		int ly = (chain_box_loc.y * 2) + 1 + dir[n][1];
		if (chessboard->Equal(lx, ly, EDGE) && GetBoxType(bx, by) == CHAIN_BOX)
		{
			if (boxes[bx][by].belonging_chain_num == EMPTY)
				RegisterCircle(chain_box_loc, NewLoc(bx,by));
		}
	}
}
void ChessBoardSolver::RegisterChain(Loc free_box_loc, Loc first_loc)
{
	//register chains info dynamically.


	int chain_num = GetEmptyChainNum();				//get a number of chain which is empty.
	chains[chain_num].start_loc = free_box_loc;		//set chain info
	chains[chain_num].total_box_num = 0;			
	chains[chain_num].chain_type = UNDEFINED;

	//prepare for searching
	Loc source_loc = free_box_loc;			
	Loc dest_loc = first_loc;	

	//in following process, the 'dest_loc' is the actual loc which is checking.
	//the type of this chain would be judged when the end of chain have been touched.
	for (int i = 0; i < BOXNUM; i++)
	{
		if (GetBoxType(dest_loc.x, dest_loc.y) != CHAIN_BOX)
		{
			//if this box is not a chain box that means it is the end of chain.
			chains[chain_num].end_loc = dest_loc;

			if (EqualLoc(free_box_loc,dest_loc)&&boxes[dest_loc.x][dest_loc.y].belonging_chain_num == EMPTY)
			{
				//if end_loc equal to start_loc and no chain own this box that means this is an OPEN_CIRCLE
				chains[chain_num].chain_type = OPEN_CIRCLE;
				boxes[dest_loc.x][dest_loc.y].belonging_chain_num = chain_num;
				chains[chain_num].total_box_num++;
			}
			else
			{
				//if end_loc and start_loc are different, this chain would be judged as a normal CHAIN.
				chains[chain_num].chain_type = CHAIN;
			}
			break;
		}
		else
		{
			//if this box is not the end of chain. 

			//define the info of this box
			boxes[dest_loc.x][dest_loc.y].belonging_chain_num = chain_num;	//the chain[num] own this box.
			chains[chain_num].total_box_num++;								//increase the boxes number of the chain[num]

			Loc next_loc = dest_loc;//find next box
			dest_loc = GetNextBox(dest_loc, source_loc);					
			source_loc = next_loc;
		}
	}
}
void ChessBoardSolver::RegisterCircle(Loc start_loc, Loc first_loc)
{
	//register circle info dynamically.

	int chain_num = GetEmptyChainNum();//set chain info
	chains[chain_num].total_box_num = 0;
	chains[chain_num].chain_type = UNDEFINED;

	//prepare for searching
	Loc source_loc = start_loc;
	Loc dest_loc = first_loc;

	for (int i = 0; i < BOXNUM; i++)
	{
		if (boxes[dest_loc.x][dest_loc.y].type != CHAIN_BOX)
		{
			//if we found a box which is not CHAIN_BOX, cancal register.
			DestoryChain(chain_num);//destory current chain.
			break;
		}
		else if (EqualLoc(dest_loc,start_loc))//if dest_loc equal to start_loc that means it is a CIRCLE
		{
			chains[chain_num].chain_type = CIRCLE;//set info
			chains[chain_num].start_loc = dest_loc;						
			chains[chain_num].end_loc = dest_loc;						

			boxes[dest_loc.x][dest_loc.y].belonging_chain_num = chain_num;//chain(num) own this box.
			chains[chain_num].total_box_num++;
			break;
		}
		else
		{
			//chain(num) own this box.
			boxes[dest_loc.x][dest_loc.y].belonging_chain_num = chain_num;
			chains[chain_num].total_box_num++;

			//find next box.
			Loc next_loc = dest_loc;
			dest_loc = GetNextBox(dest_loc, source_loc);
			source_loc = next_loc;
		}
	}
}


int ChessBoardSolver::GetEmptyChainNum()const
{
	//return an number of chain which is empty but 0.
	//chain no.0 is always empty.
	for (int i = 1; i < BOXNUM; i++)
		if (chains[i].chain_type == UNDEFINED)
			return i;
	return 0;
}
void ChessBoardSolver::InheritChain(int inheritor, int ancester)
{
	//all boxes belong to ancester would be change
	for (int j = 0; j <= BOXLEN; j++)
		for (int i = 0; i <= BOXLEN; i++)
			if (boxes[i][j].belonging_chain_num == ancester)
				boxes[i][j].belonging_chain_num = inheritor;
	chains[inheritor].total_box_num += chains[ancester].total_box_num;

	//reset the data of ancester chain
	chains[ancester].total_box_num = 0;
	chains[ancester].chain_type = UNDEFINED;
}
BoxType ChessBoardSolver::GetBoxType(sint bx, sint by) const
{
	if (bx >= 0 && bx < BOXLEN&&by >= 0 && by < BOXLEN)
		return boxes[bx][by].type;
	return FREE_BOX;
}
void ChessBoardSolver::DestoryChain(int chain_num)
{
	InheritChain(EMPTY, chain_num);
}

//test function
void ChessBoardSolver::ShowBoardInfo() const
{
	//Print Chess ChessBoardArray
	cout << "  ";
	for (int i = 0; i <= (LEN - 1); i++)
	{
		printf("%2d", i);
	}
	cout << "\n";
	for (sint j = 0; j<LEN; j++)
	{
		printf("%2d", j);
		for (sint i = 0; i<LEN; i++)
		{
			if (chessboard->Equal(i,j,BOX))
			{
				int bx = (i - 1) / 2;
				int by = (j - 1) / 2;

				if (boxes[bx][by].type == FULL_BOX)
				{
					Cprintf("■", 8);
				}
				else if (boxes[bx][by].type == DEAD_BOX)
				{
					if (boxes[bx][by].belonging_chain_num == 0)
						Cprintf("◎", 2);
					else
					{
						cout << " ";
						CprintfNum(boxes[bx][by].belonging_chain_num, 2);
					}
				}
				else if (boxes[bx][by].type == CHAIN_BOX)
				{
					if (boxes[bx][by].belonging_chain_num == 0)
						Cprintf("◎", 14);
					else
					{
						cout << " ";
						CprintfNum(boxes[bx][by].belonging_chain_num, 14);
					}
				}
				else if (boxes[bx][by].type == FREE_BOX)
				{
					if (boxes[bx][by].belonging_chain_num == 0)
						Cprintf("◎", 5);
					else
					{
						cout << " ";
						CprintfNum(boxes[bx][by].belonging_chain_num, 5);
					}
				}
				//

			}
			else if (chessboard->Equal(i, j, RED_BOX))
			{
				Cprintf("■", 12);
			}
			else if (chessboard->Equal(i, j, BLUE_BOX))
			{
				Cprintf("■", 9);
			}
			else if (chessboard->Equal(i, j, EDGE))
			{
				if (OddNum(j) && EvenNum(i))
				{
					cout << "  ";
				}
				else
				{
					cout << "  ";
				}
			}
			else if (chessboard->Equal(i, j, RED))
			{
				if (OddNum(j) && EvenNum(i))
				{
					Cprintf("┃", 12);
				}
				else
				{
					Cprintf("━", 12);
				}
			}
			else if (chessboard->Equal(i, j, BLUE))
			{
				if (OddNum(j) && EvenNum(i))
				{
					Cprintf("┃", 9);
				}
				else
				{
					Cprintf("━", 9);
				}
			}
			else if (chessboard->Equal(i, j, EMPTY))
			{
				Cprintf("○", 15);
			}
		}
		printf("%2d", j);
		cout << "\n";
	}
	cout << "  ";
	for (int i = 0; i <= (LEN - 1); i++)
	{
		printf("%2d", i);
	}
	cout << "\n";
	Cprintf("\n■ [Free Box]", 5);
	Cprintf("\n■ [Chain Box]", 14);
	Cprintf("\n■ [Dead Box]", 2);
	Cprintf("\n■ [Full Box]\n", 8);

	for (int i = 1; i < BOXNUM; i++)
	{
		if (chains[i].chain_type != UNDEFINED)
		{
			cout << "Chain[";
			CprintfNum(i, 14);
			cout<< "] BoxNum:[";
			CprintfNum(chains[i].total_box_num, 13);
			cout<< "] ChainType:[";
			if (chains[i].chain_type == CHAIN)
				Cprintf("CHAIN",2);
			if (chains[i].chain_type == CIRCLE)
				Cprintf("CIRCLE",2);
			if (chains[i].chain_type == OPEN_CIRCLE)
				Cprintf("OPEN_CIRCLE",2);
			if (chains[i].chain_type == OPEN_CHAIN)
				Cprintf("OPEN_CHAIN",2);
			if (chains[i].chain_type == DEAD_CIRCLE)
				Cprintf("DEAD_CIRCLE",2);
			if (chains[i].chain_type == DEAD_CHAIN)
				Cprintf("DEAD_CHAIN",2);
			cout << "]" << endl;
		}
	}
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