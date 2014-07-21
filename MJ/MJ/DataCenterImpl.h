#pragma once
#include "datacenter.h"
#include "DataCenterState.h"
#include "Communication.h"

#include <map>

class DataCenterImpl :
	public DataCenter
{
public:
	~DataCenterImpl(void);

public:
	static DataCenterImpl* const getInstance();

	virtual void notifyPlayer(int playerID, const CommunicateObj& commObj);
	virtual void getNotificationFromPlayer(const CommunicateObj& commObj, int playerID);

public:
	//dispatch token
	void gameStart();

	//calculate records and get ready to next round
	void gameOver(std::string msg, std::vector<Card*>* p_cards);

	//exit the game and release the resources
	void gameExit();

	//dispatch one card 
	void dispatchCard(int playerID);

	//dispatch initial cards
	void dispatchInitialCards();

	//handle the actions
	int handleAction(int playerID, Rule::ACTION action);

	//handle the incoming card, just broadcast it
	void handleIncomingCard(int playerID, Card* card);

	//change the state
	void changeState(DataCenterState* state);

	//update the token and broadcast it
	void broadCastToken(int token);


protected:
	DataCenterImpl(void);

private:
	static DataCenterImpl *theInstance;
	static ACE_Thread_Mutex dataCenterImplMutex;

private:
	void init();
	void initCards();
	void resetData();
	void broadCast(CommunicateObj* commObj);

private:
	std::vector<int> m_players; //hold the players' ids
	std::map<int, std::vector<Card*> > m_playCards; //cards in players' hands
	std::vector<Card*> m_leftCards; //the left cards
	Rule::STATE m_nState;//the current state  
	std::map<int, std::vector<Rule::ACTION> > m_actions;//the current actions

private:
	DataCenterState* m_curState;
	std::map<int, Communication*> m_communications;
};
