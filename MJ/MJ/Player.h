#pragma once


class Player
{
public:
	Player(void);
	virtual ~Player(void);
};

class HumanPlayer: public Player{
};

class RobotPlayer: public Player{
};