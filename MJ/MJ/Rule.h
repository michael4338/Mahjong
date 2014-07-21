#pragma once

struct Rule{
	static const int PLAYER_NUM = 4;
	
	enum STATE
	{
		UNAVAILABLE = 0,
		AVAILABLE = 1
	};
	static const int STATE_TOTAL = 2;

	enum ACTION
	{
		NOTHING = 0,
		CHUPAI = 1,
		PENG = 2,
		CHI = 3,
		MINGGANG = 4,
		ANGANG = 5,
		BUGANG = 6,
		HU = 7
	};
	static const int ACTION_TOTAL = 8;

	enum EVENT
	{
		NONE = 0,
		START = 1,
		TIMEOUT = 2,
		OVER = 3
	};
	static const int EVENT_TOTAL = 4;
};