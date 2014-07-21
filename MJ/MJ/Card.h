#pragma once

#include <map>

#include <ace/Synch.h>
#include <ace/Synch_T.h>
#include <ace/Method_Request.h>
#include <ace/Thread_Manager.h>

class Card
{
public:
    enum CARD_NUMBER
    {
        CARD_ZERO = 0,   
        CARD_ONE = 1,
        CARD_TWO = 2,
        CARD_THREE = 3,
        CARD_FOUR = 4,
        CARD_FIVE = 5,
        CARD_SIX = 6,
        CARD_SEVEN = 7,
        CARD_EIGHT = 8,
        CARD_NINE = 9
    };
	const static int CARD_NUMBER_TOTAL = 10;

    enum CARD_TYPE
    {
        CARD_NOTHING = 0,
        CARD_WAN = 9,
        CARD_TIAO = 18,
        CARD_BING = 27,
        CARD_FENG = 31,
        CARD_ZHONGFABAI = 34,
        CARD_HUA = 38,
        CARD_CHUNXIAQIUDONG = 42
    };
	const static int CARD_TYPE_TOTAL = 8;

	enum CARD_STATE
	{
		CARD_DEFAULT = 0,
		CARD_ENABLED = 1
	};
	const static int CARD_STATE_TOTAL = 2;

public:
	virtual ~Card(void) = 0;

    Card(CARD_TYPE type, CARD_NUMBER number);
    Card(const Card& copyCard);
    Card& operator=(const Card& copyCard);
	bool operator==(const Card& card);
	bool operator<(const Card& card);

public:
    CARD_NUMBER getCardNumber(){return m_number;}
    CARD_TYPE getCardType(){return m_type;}
	CARD_STATE getCardState(){return m_state;}
	void setCardState(CARD_STATE state){m_state = state;}
        
private:
    CARD_TYPE m_type;
    CARD_NUMBER m_number;
	CARD_STATE m_state;
};


//use Flyweight pattern for reusing the card object 
class FlyweightCard: public Card{
    
public:
    FlyweightCard(Card::CARD_TYPE type, Card::CARD_NUMBER number);
    FlyweightCard(const FlyweightCard& copyCard);
    FlyweightCard& operator=(const FlyweightCard& copyCard);
};


class CardFactory{

public:
	static CardFactory* const getInstance();
	Card* getCard(Card::CARD_TYPE type, Card::CARD_NUMBER number, Card::CARD_STATE state);

protected:
	CardFactory();
private:
	static CardFactory *m_theInstance;
	std::map<int,Card*> m_cardObjectsMap;
	typedef std::map<int,Card*>::iterator CARDMAPCI;
	static ACE_Thread_Mutex m_cardFactoryMutex;
};