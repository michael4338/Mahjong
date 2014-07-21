#pragma once

#include <vector>
#include "Rule.h"
#include "Card.h"
#include <string>
#include <sstream>

class CommunicateObj
{
public:
	enum MSGFLAG
	{
		MSG_TOKEN = 0,
		MSG_INITCARDS = 1,
		MSG_DISPCARD = 2,
		MSG_OUTCARD = 3,
		MSG_ACTION = 4,
		MSG_APPROVE = 5,
		MSG_TIMEOUT = 6,
		MSG_EVENT = 7,
		MSG_PLAYER = 8,
		MSG_GAMEOVER = 9
	};
public:        
    CommunicateObj();
    ~CommunicateObj();
    
public:
	bool serialize(char*& target);
	bool deserialize(const char* source);

public:
	MSGFLAG m_msgFlag;
	int m_nPlayerID;
	int m_nToken;
    Rule::ACTION m_action;
    Rule::EVENT m_event;
    std::vector<Card*> m_cards; 
	std::string m_msgStr;
	bool m_bApprove;
};