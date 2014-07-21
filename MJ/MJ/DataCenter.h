#pragma once

#include "Card.h"
#include "Rule.h"
#include "CommunicateObj.h"
#include <vector>

#define MAX_START_NUM 13

class DataCenter
{
public:
	DataCenter(void);
	virtual ~DataCenter(void);

public:
	virtual void notifyPlayer(int playerID, const CommunicateObj& commObj){}
	virtual void getNotificationFromPlayer(const CommunicateObj& commObj, int playerID){}

public:
	int searchCard(std::vector<Card*>& cardList, Card* card);
	bool deleteCards(int start, std::vector<Card*>& cardList, int delNum);
	bool deleteCards(std::vector<Card*>& cardList, Card* card);
	void insertCards(Card* curCard, std::vector<Card*>& cardList, int insNum);
	void sortCards(std::vector<Card*>& cardList);
	void getActions(const std::vector<Card*>& cardList, 
		Card* curCard, 
		std::vector<Rule::ACTION>& actions,
		std::vector<Card*>& outCards);
	bool checkHu(int cards[]);

protected:
	int m_nSelfID;
	std::vector<Card*> m_selfCards; //cards in players' hands
	std::map<int, std::vector<Card*> > m_pushCards; //cards pushed and saved by players
	std::map<int, std::vector<Card*> > m_outCards; //cards send out on the table
	Card* m_lastCard; //the card send out onto table
	int m_nLastPlayer;//the player who send out card currently
	int m_nToken;//the player on action
	Rule::ACTION m_action;//the current action
};

