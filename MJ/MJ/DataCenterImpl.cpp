#include "StdAfx.h"
#include "DataCenterImpl.h"
#include "NetCommunication.h"
#include "DataCenterState.h"

DataCenterImpl* DataCenterImpl::theInstance = 0;
ACE_Thread_Mutex DataCenterImpl::dataCenterImplMutex;


extern Card* getCardBySerial(int serial);

DataCenterImpl::DataCenterImpl(void):m_curState(0)
{
}

DataCenterImpl::~DataCenterImpl(void)
{
}

DataCenterImpl* const DataCenterImpl::getInstance()
{
	if(theInstance == 0)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(dataCenterImplMutex);
		if(guard.locked() == 0) 
		    return 0;
		if(theInstance == 0)
		{
			theInstance = new DataCenterImpl();
		}
	}
	return theInstance;
}

void DataCenterImpl::notifyPlayer(int playerID, const CommunicateObj& commObj)
{
	if ( (int) m_communications.size() > 0 && m_communications[playerID] )
		m_communications[playerID]->send(commObj);
}

void DataCenterImpl::getNotificationFromPlayer(const CommunicateObj& commObj, int playerID)
{
}

//suppose to do init only once for one table of players
void DataCenterImpl::init()
{
	//set start state
	changeState(DataCenterStartState::getInstance());
	
	//init player IDs, start from 0
	std::vector<Card*> cardVec;
	std::vector<Rule::ACTION> actionVec;
	for ( int i = 0; i < Rule::PLAYER_NUM; i++ )
	{
		m_players[i] = i;
		m_playCards[i] = cardVec;
		m_actions[i] = actionVec;
		Communication* comm = new NetCommunication(true, 3333);
		comm->init();
		m_communications[i] = comm;
	}
	resetData();
}

void DataCenterImpl::gameStart()
{
	//dispatch token
	m_nToken = rand() % Rule::PLAYER_NUM;

	//broadcast token to every one
	CommunicateObj commObj;
	commObj.m_msgFlag = CommunicateObj::MSG_TOKEN;
	commObj.m_nToken = m_nToken;

	broadCast(&commObj);
}

void DataCenterImpl::gameOver(std::string msg, std::vector<Card*>* p_cards)
{
	//broadcast to every one that the game is over
	CommunicateObj commObj;
	commObj.m_msgFlag = CommunicateObj::MSG_GAMEOVER;
	commObj.m_nToken = m_nToken;
	commObj.m_msgStr = msg;

	broadCast(&commObj);

	//reset every member to be the default one
	resetData();
}

void DataCenterImpl::dispatchCard(int playerID)
{
	int leftCardNum = m_leftCards.size(); 
	if ( leftCardNum > 0 )
	{
		Card* theCard = m_leftCards[leftCardNum-1];
		m_leftCards.pop_back();
		m_playCards[playerID].push_back(theCard);

		m_nToken = playerID;
		m_lastCard = theCard;

		CommunicateObj commObj;
		commObj.m_msgFlag = CommunicateObj::MSG_DISPCARD;
		commObj.m_cards.push_back(theCard);

		notifyPlayer(playerID, commObj);
	}
	else
	{
		std::vector<Card*> vec;
		gameOver("no cards left", &vec);
	}
}

void DataCenterImpl::dispatchInitialCards()
{
	initCards();
	for ( int i = 0; i < (int) m_players.size(); i++ )
	{
		int curPlayerID = m_players[i];

		CommunicateObj commObj;
		commObj.m_msgFlag = CommunicateObj::MSG_INITCARDS;

		for ( int j = 0; j < (int) m_playCards[curPlayerID].size(); j++ )
			commObj.m_cards.push_back(m_playCards[curPlayerID][j]);

		notifyPlayer(curPlayerID, commObj);
	}
}

/*
return value:
-1: action is rejected
0: action is approved and not HU
1: action is approved and is HU
*/
int 
DataCenterImpl::
handleAction(int playerID, Rule::ACTION action)
{
	std::map<int, std::vector<Card*> > m_pushCards; //cards pushed and saved by players
	std::map<int, std::vector<Card*> > m_outCards; //cards send out on the table
	std::map<int, std::vector<Card*> > m_playCards; //cards in players' hands




	std::vector<Rule::ACTION> actions;
	std::vector<Card*> cards;

	getActions(m_playCards[playerID], m_lastCard, actions, cards);

	bool found = false; 
	for ( int i = 0; i < (int)actions.size(); i ++ )
	{
		if ( actions[i] == action )
		{
			found = true;
			break;
		}
	}
	
	int returnValue = -1;

	CommunicateObj commObj;
	commObj.m_msgFlag = CommunicateObj::MSG_APPROVE;
	commObj.m_bApprove = found;
	commObj.m_nToken = m_nToken = ( found ? playerID : m_nToken );
	commObj.m_action = action;

	if ( ! found )
	{
		returnValue = -1;
		notifyPlayer(playerID, commObj);
	}
	else if ( action == Rule::HU )
	{
		if ( m_nToken != playerID )//dian pao
		{
			deleteCards(m_outCards[m_nToken], m_lastCard);
			m_playCards[playerID].push_back(m_lastCard);
			sortCards(m_playCards[playerID]);
		}

		std::vector<Card*> winnerCards;
		for ( int i = 0; i < (int) m_playCards[playerID].size(); i++ )
			winnerCards.push_back(m_playCards[playerID][i]);

		gameOver("we have a winner!", &winnerCards);

		returnValue = 1;
	}
	else if ( action == Rule::PENG )
	{
		commObj.m_cards.push_back( m_lastCard );
		
		for ( int i = 0; i < 2; i ++ )
		{
			deleteCards(m_playCards[playerID], m_lastCard);
			m_pushCards[playerID].push_back(m_lastCard);
		}
		deleteCards(m_outCards[m_nToken], m_lastCard);
		m_pushCards[playerID].push_back(m_lastCard);

		returnValue = 0;
	}
	else if ( action == Rule::ANGANG )
	{
		commObj.m_cards.push_back( cards[0] );
		for ( int i = 0; i < 4; i ++ )
		{
			deleteCards( m_playCards[playerID], cards[0] );
			m_pushCards[playerID].push_back( cards[0] );
		}

		returnValue = 0;
	}
	else if ( action == Rule::MINGGANG )
	{
		commObj.m_cards.push_back( m_lastCard );
		for ( int i = 0; i < 3; i ++ )
		{
			deleteCards(m_playCards[playerID], m_lastCard);
			m_pushCards[playerID].push_back(m_lastCard);
		}
				
		deleteCards(m_outCards[m_nToken], m_lastCard);
		m_pushCards[playerID].push_back(m_lastCard);

		returnValue = 0;
	}
	else if ( action == Rule::BUGANG )
	{
		commObj.m_cards.push_back( m_lastCard );
		for ( int i = 0; i < 3; i ++ )
		{
			deleteCards(m_outCards[playerID], m_lastCard);
			m_pushCards[playerID].push_back(m_lastCard);
		}
				
		deleteCards(m_playCards[playerID], m_lastCard);
		m_pushCards[playerID].push_back(m_lastCard);

		returnValue = 0;
	}
	else if ( action == Rule::CHI )
	{
		commObj.m_cards.push_back(m_lastCard);
		for ( int i = 0; i < (int)cards.size(); i ++ )
		{
			deleteCards(m_playCards[playerID], cards[i]);
			commObj.m_cards.push_back(cards[i]);
			m_pushCards[playerID].push_back(cards[i]);
		}
		deleteCards(m_outCards[m_nToken], m_lastCard);
		m_pushCards[playerID].push_back(m_lastCard);
		sortCards(m_pushCards[playerID]);

		returnValue = 0;
	}

	return returnValue;
}

void DataCenterImpl::handleIncomingCard(int playerID, Card* card)
{
	if ( ! deleteCards(m_playCards[playerID], card) )
	{
		std::vector<Card*> vec;
		gameOver("unexpected card received", &vec);
		return;
	}

	m_lastCard = card;
	m_outCards[playerID].push_back(card);
	m_nLastPlayer = playerID;
	m_nToken = playerID;

	CommunicateObj commObj;
	commObj.m_msgFlag = CommunicateObj::MSG_OUTCARD;
	commObj.m_nToken = m_nToken;

	broadCast(&commObj);
}

void DataCenterImpl::broadCastToken(int token)
{
	m_nToken = token;

	CommunicateObj commObj;
	commObj.m_msgFlag = CommunicateObj::MSG_TOKEN;
	commObj.m_nToken = m_nToken;

	broadCast(&commObj);
}

void DataCenterImpl::changeState(DataCenterState* state)
{
	m_curState = state;
}
	
void DataCenterImpl::initCards()
{
	const int totalCardNum = Card::CARD_CHUNXIAQIUDONG * 4;

	std::map<int, std::vector<Card*> >::iterator iter;

	bool* sendCardFlag = new bool[totalCardNum];
	memset (sendCardFlag, 0, totalCardNum);

	Card* theCard = 0;
	int serial = 0;

	//generate initial cards for everybody
	for ( iter = m_playCards.begin(); iter != m_playCards.end(); iter++ )
	{
		iter->second.clear();

		for (int i=0; i<MAX_START_NUM; i++)
		{
			serial = 0;
			do
			{
				serial = rand() % totalCardNum;
			}
			while (sendCardFlag[serial]);
			sendCardFlag[serial] = true;

			if ( theCard = getCardBySerial(serial) )
				iter->second.push_back(theCard);
		}

		sortCards(iter->second);
	}

	//send the last card to "zhuang"
	serial = 0;
	do
	{
		serial = rand() % totalCardNum;
	}
	while(sendCardFlag[serial]);
	sendCardFlag[serial] = true;

	theCard = getCardBySerial(serial);

	if(theCard)
	{
		m_playCards[m_nToken].push_back(theCard);
		sortCards(m_playCards[m_nToken]);
	}

	//initialize the left cards
	int i = 0;
	m_leftCards.clear();
	while( i++ <  (int)(totalCardNum-m_playCards.size()*MAX_START_NUM-1) )
	{
		serial = 0;
		do
		{
			serial = rand() % totalCardNum;
		}
		while(sendCardFlag[serial]);
		sendCardFlag[serial] = true;

		theCard = getCardBySerial(serial);
		if(theCard)
			m_leftCards.push_back(theCard);
	}

	delete []sendCardFlag;
}

void DataCenterImpl::resetData()
{
	for ( int i = 0; i < (int) m_players.size(); i++ )
	{
		m_playCards[i].clear(); 
		m_actions[i].clear();
		m_pushCards[i].clear();
		m_outCards[i].clear();
	}
	m_nState = Rule::AVAILABLE; 
	m_leftCards.clear(); 
	m_lastCard = 0;
	m_nLastPlayer = 0;
	m_nToken = 0;
}

void DataCenterImpl::broadCast(CommunicateObj* commObj)
{
	for ( int i = 0; i < (int) m_players.size(); i++ )
	{
		commObj->m_nPlayerID = m_players[i];
		notifyPlayer(i, *commObj);
	}
}
