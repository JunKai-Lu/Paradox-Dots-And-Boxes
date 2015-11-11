#pragma once

#include "stdafx.h"
#include "windows.h"
#include <iostream>

enum ChainType
{
	NOT_DEFINE, SINGLE_CHAIN, SHORT_CHAIN, LONG_CHAIN, PRE_LONG_CHAIN, CIRCLE, PRE_CIRCLE, DEAD_CHAIN, DEAD_CIRCLE
};

enum BoxType
{
	CPUTURED, DEAD_BOX, CHAIN_BOX, FREE_BOX
};

class BoxInfo
{
public:
	BoxType Type;				//include CPUTURED, DEAD_BOX, CHAIN_BOX, FREE_BOX
	int BelongingChainNum;		//the number of the chain which this box belongs.
};

class ChainInfo
{
public:
	ChainInfo()
	{
		chain_type = NOT_DEFINE;
		total_box_num = 0;
	}
	ChainType chain_type;
	int total_box_num;
	Loc start_loc;
	Loc end_loc;
};