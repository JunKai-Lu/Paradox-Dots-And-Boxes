/* 
This is an BASIC VERSION of dots-and-boxes game program which is aimed to be the solver of this game.

In this version we use MCTS ALGORITHM and bulid an searching tree with a searching bias.

More infomation, please read README.TXT which included in this project.
*/

#include "stdafx.h"
#include "GameDefine.h"
#include <iostream>
#include "windows.h"
#include "time.h"
#include "math.h"

using namespace std;

void Model_AI_Game()
{
	int model;
	char red_name[20];
	char blue_name[20];
	void(*func_red)(BOARD&, int, bool);
	void(*func_blue)(BOARD&, int, bool);
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
		BOARD NewBoard;
		NewBoard.PrintBoard();
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

			//w = GetBoardWinner(NewBoard, -player);
			w = NewBoard.Winner();

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

				func_red(NewBoard, player, true);
			}
			else
			{

				func_blue(NewBoard, player, true);
			}*/
			t_finish = clock();
			t_totaltime = (double)(t_finish - t_start) / CLOCKS_PER_SEC;
			cout << "\n消耗时间为" << t_totaltime << "秒！" << endl;
			player = -player;//玩家切换
			NewBoard.PrintBoard();//显示局面
			w = NewBoard.Winner();
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
void Model_Board_Winner_Test()
{
	while (1)
	{
		cout << "New Test Start" << endl;
		BOARD Test;
		//Test.LoadBoard();
		int r =1;// = GetBoardWinner(Test, BLUE);
		int b =0;// = GetBoardWinner(Test, RED);
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
void Model_Define_Board(BOARD &CB)
{
	//CB.LoadBoard();
	cout << "Define Finish!" << endl;
	system("pause");
	system("CLS");

}
void Model_Define_Chain()
{
	while (1)
	{
		cout << "New Test Start" << endl;
		BOARD Test;
		//Test.LoadBoard();
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
	Cprintf("Less Memory, More Effective", 8);
}
int _tmain(int argc, _TCHAR* argv[])
{
	//Definition
	clock_t start, finish;			//Timer Setup
	double totaltime;				//Timer Setup
	srand((unsigned)time(NULL));	//Srand 
	Info();							//Show Game Infomation
	BOARD CB;						//Create a new chess board;

	//Choose Model
	int model;
	cin >> model;
	if (model == 0)
		Cprintf("Open Default Model", 8);
	else if (model == 1)
		Model_AI_Game();
	else if (model == 2)
		Model_Define_Chain();
	else if (model == 3)
		Model_Board_Winner_Test();
	else
		Model_Define_Board(CB);


	for (;;)
	{
		//check winner at first(because the game may start from a state which exist a winner)
		int W = CB.Winner();
		if (W == RED)
		{
			cout << "Red Win" << endl;
			CB.PrintBoard();
			system("pause");
			break;
		}
		if (W == BLUE)
		{
			cout << "Blue Win。" << endl;
			CB.PrintBoard();
			system("pause");
			break;
		}

		//take turns to move
		CB.PrintBoard();
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
			CB.PrintBoard();
			cout << "\nInput LOCATION and PLAYER，such as \"2,1,1\"" << endl;
			for (;;)
			{
				int locx, locy, owner;
				scanf_s("%d,%d,%d", &locx, &locy, &owner);
				if ((owner == 1 || owner == 2) && locx >= 0 && locx <= (LEN - 1) && locy >= 0 && locy <= (LEN - 1))//limit the range of input
				{
					if (owner == 2)
						owner = -1;
					if ((OddNum(locx) && EvenNum(locy)) || (EvenNum(locx) && OddNum(locy)))//'x' and 'y' should be a odd number and a even number
					{
						MOVE m = NewMove(locx, locy, owner);
						CB.Move(m, true);
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

