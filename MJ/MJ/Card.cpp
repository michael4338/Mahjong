#include "StdAfx.h"
#include "Card.h"

Card::~Card(void)
{
}

Card::Card(Card::CARD_TYPE type, Card::CARD_NUMBER number):m_type(type), m_number(number), m_state(CARD_DEFAULT)
{
}

Card::Card(const Card& copyCard):m_type(copyCard.m_type),m_number(copyCard.m_number),m_state(copyCard.m_state)
{
}

Card& Card::operator=(const Card& copyCard)
{
    m_type = copyCard.m_type;
    m_number = copyCard.m_number;
	m_state = copyCard.m_state;
    return *this;
}

bool Card::operator==(const Card& card)
{
	return (m_type == card.m_type && m_number == card.m_number);
}

bool Card::operator<(const Card& card)
{
	return ( m_type < card.m_type || (m_type == card.m_type && m_number < card.m_number) );
}


FlyweightCard::FlyweightCard(Card::CARD_TYPE type, Card::CARD_NUMBER number):Card(type, number)
{
}

FlyweightCard::FlyweightCard(const FlyweightCard& copyCard):Card(copyCard)
{
}
    
FlyweightCard& FlyweightCard::operator=(const FlyweightCard& copyCard)
{
    Card::operator=(copyCard);
	return *this;
}


CardFactory* CardFactory::m_theInstance = 0;
ACE_Thread_Mutex CardFactory::m_cardFactoryMutex;

CardFactory::CardFactory()
{
}

CardFactory* const CardFactory::getInstance()
{
	if(m_theInstance == 0)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(m_cardFactoryMutex);
		if(guard.locked() == 0) 
		    return 0;
		if(m_theInstance == 0)
		{
			m_theInstance = new CardFactory();
		}
	}
	return m_theInstance;
}

Card* CardFactory::getCard(Card::CARD_TYPE type, Card::CARD_NUMBER number, Card::CARD_STATE state)
{
    ACE_Guard<ACE_Thread_Mutex> guard(m_cardFactoryMutex);
    if(guard.locked() == 0) 
        return 0;

	if(type == Card::CARD_NOTHING || number == Card::CARD_ZERO)
		return 0;

	int serial = type*(Card::CARD_NUMBER_TOTAL)*(Card::CARD_STATE_TOTAL) + number*(Card::CARD_STATE_TOTAL) + state;

	CARDMAPCI iter = m_cardObjectsMap.find(serial);
    bool found = (iter != m_cardObjectsMap.end());

	if(!found) 
	{
		Card* newCard = new FlyweightCard(type, number);
		m_cardObjectsMap[serial] = newCard;
	}
		
	return m_cardObjectsMap[serial];
}
