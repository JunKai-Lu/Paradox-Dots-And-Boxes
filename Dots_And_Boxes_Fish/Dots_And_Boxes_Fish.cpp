/* 
This is an BASIC VERSION of dots-and-boxes game program which is aimed to be the solver of this game.

In this version we use MCTS ALGORITHM and bulid an searching tree with a searching bias.

More infomation, please read README.TXT which included in this project.
*/

#include "stdafx.h"
#include <iostream>
#include "windows.h"
#include "time.h"
#include "math.h"

#include "GameDefine.h"
#include "StateAnalysis.h"
#include "SearchAlgorithm.h"

using namespace std;

void Model_AI_Game()
{
	int model;
	char red_name[20];
	char blue_name[20];
	void(*func_red)(ChessBoard&, int, bool);
	void(*func_blue)(ChessBoard&, int, bool);
	clock_t t_start, t_finish;		//Timer Setup
	double t_totaltime;				//Timer Setup
	srand((unsigned)time(NULL));	//Srand


	//Choose Algorithm
	for (;;)
	{
		Cprintf("RED Algorithm\n[1. UCT Algorithm]", 9);
		cin >> model;
		if (model >= 1 && model <= 1)
		{
			if (model == 1)
			{
				//func_red = GameTurnMove;
				strcpy_s(red_name, "GameTurnMove");
			}
			break;
		}
		else
		{
			Cprintf("Wrong Input", 8);
		}
	}
	for (;;)
	{
		Cprintf("BLUE Algorithm\n[1. UCT Algorithm]", 9);
		cin >> model;
		if (model >= 1 && model <= 1)
		{
			if (model == 1)
			{
				//func_blue = GameTurnMove;
				strcpy_s(blue_name, "GameTurnMove");
			}
			break;
		}
		else
		{
			Cprintf("Wrong Input", 8);
		}
	}

	//Game Start
	int red_count = 0, blue_count = 0;
	for (int n = 1; n<200; n++)
	{
		//system("pause");
		ChessBoard NewChessBoardArray;
		NewChessBoardArray.PrintCB();
		int player = 1 - (2 * (n % 2));//take turns to be the first one
		cout << "[GAME] No" << n << " Have Began. ";
		printf("%s Move First", player == RED ? "RED" : "BLUE");
		for (;;)
		{
			int w = 0, tz = 0;
			if (player == RED)
				Cprintf("RED ", 12);
			else
				Cprintf("BLUE ", 9);
			cout << "GAME No[" << n << "]，";
			Cprintf(red_name, 12);
			cout << "Win[" << red_count << "]";
			Cprintf(blue_name, 9);
			cout << "Win[" << blue_count << "]" << endl;

			//w = GetChessBoardArrayWinner(NewChessBoardArray, -player);
			w = NewChessBoardArray.Winner();

			if (w == RED)
			{
				red_count++;
				break;
			}
			if (w == BLUE)
			{
				blue_count++;
				break;
			}
			t_start = clock();
			/*if (player == RED)
			{

				func_red(NewChessBoardArray, player, true);
			}
			else
			{

				func_blue(NewChessBoardArray, player, true);
			}*/
			t_finish = clock();
			t_totaltime = (double)(t_finish - t_start) / CLOCKS_PER_SEC;
			cout << "\n消耗时间为" << t_totaltime << "秒！" << endl;
			player = -player;//玩家切换
			NewChessBoardArray.PrintCB();//显示局面
			w = NewChessBoardArray.Winner();
			if (w == RED)
			{
				red_count++;
				break;
			}
			if (w == BLUE)
			{
				blue_count++;
				break;
			}
		}
	}
	cout << "ALL GAME FINISH\n\n\nRED Win" << red_count << "    BLUE Win" << blue_count << endl;
}
void Model_ChessBoardArray_Winner_Test()
{
	while (1)
	{
		cout << "New Test Start" << endl;
		ChessBoard Test;
		//Test.LoadChessBoardArray();
		int r =1;// = GetChessBoardArrayWinner(Test, BLUE);
		int b =0;// = GetChessBoardArrayWinner(Test, RED);
		if (r || b)
		{
			cout << "The Winner is";
			if (r == RED)
				Cprintf("RED", 12);
			else
				Cprintf("BLUE", 9);
			cout << "if [RED] move in next turn!" << endl;

			cout << "The Winner is";
			if (b == RED)
				Cprintf("RED", 12);
			else
				Cprintf("BLUE", 9);
			cout << "if [BLUE] move in next turn!"<<endl;
		}
		cout << endl;
		system("pause");
	}
}
void Model_Define_ChessBoardArray(ChessBoard &CB)
{
	//CB.LoadChessBoardArray();
	cout << "Define Finish!" << endl;
	system("pause");
	system("CLS");

}
void Model_Define_Chain()
{
	while (1)
	{
		cout << "New Test Start" << endl;
		ChessBoard Test;
		//Test.LoadChessBoardArray();
		cout << "New Test Start" << endl;
		//GameTurnMove(Test, RED, true);
		system("pause");
		system("CLS");
	}
}
void Info()
{
	Cprintf("[Paradox Fish Dots-and-Boxes Engine]\n\n", 14);
	Cprintf("		Version:1.0.1. \n\n", 11);
	Cprintf("[  0  ]	Default Game Model \n[  1  ]	Agorithm Testing\n[  2  ]	Testing Model\n[  3  ]	Winner Testing\n[other]	Define Game\n\n", 10);
	Cprintf("Less Memory, More Effective\n\n", 8);
}
int _tmain(int argc, _TCHAR* argv[])
{
	//Definition
	clock_t start, finish;			//Timer Setup
	double totaltime;				//Timer Setup
	srand((unsigned)time(NULL));	//Srand 
	Info();							//Show Game Infomation
	ChessBoard CB;					//Create a new chess board;

	/*Time Test
	start = clock();

	MctsNode HH(CB, RED);
	int n = 0;
	for (int i = 0; i < 300000; i++)
	{
		if (HH.GetMovesWithBias() == 60)
			n++;
	}
	
	

	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\nThis Turn cost [" << totaltime << "] Seconds!" << endl;
	
	*/
	

	//Test
	Move a;
	a.Set(0, 1, RED);
	CB.GameMove(a, false);
	a.Set(1, 0, RED);
	CB.GameMove(a, false);
	a.Set(1, 2, RED);
	CB.GameMove(a, false);

	MctsNode HH(CB, RED);
	HH.SingleSimulation();
	
	//Choose Model
	int model;
	cin >> model;
	if (model == 0)
		Cprintf("Open Default Model\n\n", 8);
	else if (model == 1)
		Model_AI_Game();
	else if (model == 2)
		Model_Define_Chain();
	else if (model == 3)
		Model_ChessBoardArray_Winner_Test();
	else
		Model_Define_ChessBoardArray(CB);


	for (;;)
	{
		//check winner at first(because the game may start from a state which exist a winner)
		int W = CB.Winner();
		if (W == RED)
		{
			cout << "Red Win" << endl;
			CB.PrintCB();
			system("pause");
			break;
		}
		if (W == BLUE)
		{
			cout << "Blue Win。" << endl;
			CB.PrintCB();
			system("pause");
			break;
		}

		//take turns to move
		CB.PrintCB();
		cout << "\n【0】Human  【1】AI" << endl;
		int com;
		for (;;)
		{
			int inputnumber;
			cin >> inputnumber;
			if (inputnumber == 0 || inputnumber == 1)
			{
				com = inputnumber;
				break;
			}
			Cprintf("WRONG ORDER, Please input again", 8);
		}
		if (com == 0)
		{
			system("cls");
			CB.PrintCB();
			for (;;)
			{
				cout << "\nInput LocATION and PLAYER，such as \"2 1 1\"" << endl;
				int locx, locy, owner;
				scanf_s("%d %d %d", &locx, &locy, &owner);
				if ((owner == 1 || owner == 2) && locx >= 0 && locx <= (LEN - 1) && locy >= 0 && locy <= (LEN - 1))//limit the range of input
				{
					if (owner == 2)
						owner = -1;
					if ((OddNum(locx) && EvenNum(locy)) || (EvenNum(locx) && OddNum(locy)))//'x' and 'y' should be a odd number and a even number
					{
						Move m = NewMove(locx, locy, owner);
						CB.GameMove(m, true);
						break;
					}
					else
					{
						cout << "WRONG ORDER" << endl;
					}
				}
				Cprintf("WRONG ORDER, Please input again\n", 8);
			}
		}
		else if (com == 1)
		{
			cout << "Please Input Player (1/2)" << endl;
			for (;;)
			{
				int owner;
				cin >> owner;
				if (owner == RED || owner == 2)
				{
					if (owner == 2)
						owner = BLUE;
					start = clock();
					//GameTurnMove(CB, owner, true);//AI Move
					finish = clock();
					totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
					cout << "\nThis Turn cost [" << totaltime << "] Seconds!" << endl;
					break;
				}
				Cprintf("WRONG ORDER, Please input again\n", 8);
			}
		}
	}
	return 0;
}