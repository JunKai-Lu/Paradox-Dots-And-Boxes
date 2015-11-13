#pragma once

#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include "GameDefine.h"

enum ChainType
{
	NOT_DEFINE, SINGLE_CHAIN, SHORT_CHAIN, LONG_CHAIN, PRE_LONG_CHAIN, CIRCLE, PRE_CIRCLE, DEAD_CHAIN, DEAD_CIRCLE
};

enum BoxType
{
	FULL_BOX = 0, DEAD_BOX = 1, CHAIN_BOX = 2, FREE_BOX = 3
};

class BoxInfo
{
public:
	BoxType type;				//include CPUTURED, DEAD_BOX, CHAIN_BOX, FREE_BOX
	int belonging_chain_num;	//the number of the chain which this box belong.
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

