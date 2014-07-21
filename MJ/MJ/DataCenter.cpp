#include "StdAfx.h"
#include "DataCenter.h"

DataCenter::DataCenter(void):m_nSelfID(0), m_lastCard(0), m_nLastPlayer(0), m_nToken(0)
{
}
DataCenter::~DataCenter(void)
{
}

Card* getCardBySerial(int serial)
{
	Card::CARD_TYPE type = Card::CARD_NOTHING;
	Card::CARD_NUMBER number = Card::CARD_ZERO;

	if ( serial>=Card::CARD_NOTHING*4 && serial<Card::CARD_WAN*4 )
	{
		type = Card::CARD_WAN;
		number = (Card::CARD_NUMBER)(serial/4-Card::CARD_NOTHING + 1);
	}
	else if ( serial>=Card::CARD_WAN*4 && serial<Card::CARD_TIAO*4 )
	{
		type = Card::CARD_TIAO;
		number = (Card::CARD_NUMBER)(serial/4-Card::CARD_WAN+1);
	}
	else if ( serial>=Card::CARD_TIAO*4 && serial<Card::CARD_BING*4 )
	{
		type = Card::CARD_BING;
		number = (Card::CARD_NUMBER)(serial/4-Card::CARD_TIAO+1);
	}
	else if ( serial>=Card::CARD_BING*4 && serial<Card::CARD_FENG*4 )
	{
		type = Card::CARD_FENG;
		number = (Card::CARD_NUMBER)(serial/4-Card::CARD_BING+1);
	}
	else if ( serial>=Card::CARD_FENG*4 && serial<Card::CARD_ZHONGFABAI*4 )
	{
		type = Card::CARD_ZHONGFABAI;
		number = (Card::CARD_NUMBER)(serial/4-Card::CARD_FENG+1);
	}
	else if ( serial>=Card::CARD_ZHONGFABAI*4 && serial<Card::CARD_HUA*4 )
	{
		type = Card::CARD_HUA;
		number = (Card::CARD_NUMBER)(serial/4-Card::CARD_ZHONGFABAI+1);
	}
	else if ( serial>=Card::CARD_HUA*4 && serial<Card::CARD_CHUNXIAQIUDONG*4 )
	{
		type = Card::CARD_CHUNXIAQIUDONG;
		number = (Card::CARD_NUMBER)(serial/4-Card::CARD_HUA+1);
	}

	CardFactory* instance = CardFactory::getInstance();
	if ( instance == 0 )
		return 0;

	return instance->getCard(type, number, Card::CARD_DEFAULT);
}

void singlePushBack(std::vector<Rule::ACTION>& actionList, Rule::ACTION item)
{
	bool found = false;

	for ( int i=0; i<(int)actionList.size(); i++ )
	{
		if ( actionList[i] == item )
		{
			found = true;
			break;
		}
	}
	if ( !found )
		actionList.push_back(item);
			
}

void convertVecToArray(std::vector<Card*>& vec, int cards[], int length)
{
	//the example finally built
	/*
	int cards[46] = { 0,
                    1,1,1,0,1,1,1,0,0,                    // cards[ 1- 9]  1WAN-9WAN
                    0,
                    0,0,0,0,0,3,0,0,0,                    // cards[11-19]  1TIAO-9TIAO
                    0,
                    0,0,0,0,0,0,0,0,0,                    // cards[21-29]  1BING-9BING
                    0,
                    0,1,1,1,0,0,0,0,0,0,0,0,0,0,0         // cards[31-45]  FENGHUAXUEYUE
                    };
	*/

	if ( length <= 30 )
		return;

	cards[0] = cards[10] = cards[20] = cards[30] = 0;
	
	for ( int i = 0; i < (int) vec.size(); i ++ )
	{
		int serial = (int) vec[i]->getCardNumber();

		switch ( vec[i]->getCardType() )
		{
		case Card::CARD_WAN:
			serial += 0;
			break;
		case Card::CARD_TIAO:
			serial += 10;
			break;
		case Card::CARD_BING:
			serial += 20;
			break;
		case Card::CARD_FENG:
			serial += 30;
			break;
		case Card::CARD_ZHONGFABAI:
			serial += 30 + 4;
			break;
		case Card::CARD_HUA:
			serial += 30 + 4 + 3;
			break;
		case Card::CARD_CHUNXIAQIUDONG:
			serial += 30 + 4 + 3 + 4;
			break;
		default:
			break;
		}
					
		cards[serial] = 1;
	}
}

// check remain card number
int remain(int cards[])
{
    int sum = 0;
    for ( int i=0; i<Card::CARD_CHUNXIAQIUDONG; i++ )
        sum += cards[i];
    return sum;
}

int DataCenter::searchCard(std::vector<Card*>& cardList, Card* card)
{
	for ( int i=0; i<(int)cardList.size(); i++ )
		if ( *cardList[i] == *card )
			return i;
	return -1;
}

bool DataCenter::deleteCards(int start, std::vector<Card*>& cardList, int delNum)
{
	int listLen = cardList.size();

	if ( start >= listLen )
		return false;

	std::vector<Card*>::iterator iter;
	for ( int i=0; i<start; i++ )
		iter ++;

	for ( int i=0; i<delNum && (i+start)<listLen; i++ )
	{
		iter = cardList.erase(iter);
		return true;
	}

	return false;
}

bool DataCenter::deleteCards(std::vector<Card*>& cardList, Card* card)
{
	int pos = -1;
	if ( -1 != ( pos = searchCard(cardList, card) ) ) 
		return deleteCards(pos, cardList, 1);
	return false;
}
	
void DataCenter::insertCards(Card* curCard, std::vector<Card*>& cardList, int insNum)
{
	for ( int i=0; i<insNum; i++ )
		cardList.push_back(curCard);
	sortCards(cardList);
}

void DataCenter::sortCards(std::vector<Card*>& cardList)
{
	for ( int i=0; i<(int)cardList.size()-1; i++ )
    {
		int j = i;
		for ( int k=i+1; k<(int)cardList.size(); k++ )
			if ( *cardList[k] < *cardList[j] )
				j=k;
		if ( j != i )
		{
			Card* swap = cardList[j];
			cardList[j] = cardList[i];
			cardList[i] = swap;
		}
	}
}

bool DataCenter::checkHu(int cards[])
{
    static bool JIANG = false;

    if ( !remain(cards) ) 
		return true;

	int i = 0;
    for ( i=0; !cards[i] && i<Card::CARD_CHUNXIAQIUDONG; i++ );

    // 4 (gang)
    if ( cards[i] == 4 )               
    {
        cards[i] = 0;                  
        if ( checkHu(cards) ) 
			return true;      
        cards[i] = 4;                 
    }

    // 3 (san)
    if ( cards[i] >= 3 )               
    {
        cards[i] -= 3;                 
        if ( checkHu(cards) ) 
			return true;      
        cards[i] += 3;                 
    }

    // 2 (jiang)
    if ( !JIANG && cards[i] >= 2 )   
    {
        JIANG = true;                   
        cards[i] -= 2;                 
        if ( checkHu(cards) ) 
			return true;      
        cards[i] += 2;                 
        JIANG = false;                      
	}

    
	if ( i > Card::CARD_BING )    
		return false;    

    // (fu, in sequence)
	if( i%Card::CARD_NUMBER_TOTAL != Card::CARD_EIGHT 
		&& i%Card::CARD_NUMBER_TOTAL != Card::CARD_NINE  
		&& cards[i+1]
		&& cards[i+2] )
    {
        cards[i]--;
        cards[i+1]--;
        cards[i+2]--;                  
        if ( checkHu(cards) ) 
			return true;     
        cards[i]++;
        cards[i+1]++;
        cards[i+2]++;                
    }

    return false;
}

/* parameters
** cardList: input cards
** curCard: the special input card
** actions: output, the gotten actions
** cards: output, the related cards for different actions
*/
void 
DataCenter::
getActions(const std::vector<Card*>& cardList,
		   Card* curCard, 
		   std::vector<Rule::ACTION>& actions,
		   std::vector<Card*>& outCards)
{
	//check ming gang
	for ( int i=0; i<(int)cardList.size()-2; i++ )
	{
		if ( *curCard == *cardList[i]
			&& *curCard == *cardList[i+1]
			&& *curCard == *cardList[i+2] ) 
		{
			singlePushBack(actions, Rule::MINGGANG);
			break;
		}
	}

	//check an gang
	if ( m_nSelfID == m_nToken )
	{
		for ( int i=0; i<(int)cardList.size()-3; i++ )
		{
			if ( *cardList[i] == *cardList[i+1] 
				&& *cardList[i] == *cardList[i+2] 
				&& *cardList[i] == *cardList[i+3] ) 
			{
				singlePushBack(actions, Rule::ANGANG);
				outCards.push_back(cardList[i]);
				break;
			}
		}
	}

	//check bu gang
	if ( m_nSelfID == m_nToken )
	{
		for ( int i=0; i<(int)m_pushCards[m_nSelfID].size()-2; i++ )
		{
			if( *curCard == *m_pushCards[m_nSelfID][i]
				&& *curCard == *m_pushCards[m_nSelfID][i+1]
				&& *curCard == *m_pushCards[m_nSelfID][i+2] )
			{
				singlePushBack(actions, Rule::BUGANG);
				break;
			}
		}
	}

	//check peng
	for( int i=0; i<(int)cardList.size()-1; i++ )
		if(*curCard == *cardList[i]
			&& *curCard == *cardList[i+1])
		{
			singlePushBack(actions, Rule::PENG);
			break;
		}
	
	//check chi

	bool nextPlayer = false;
	if ( (m_nToken + 1) % Rule::PLAYER_NUM == m_nSelfID )
		nextPlayer = true;
	if ( nextPlayer )
	{
		//1 1 2 2 3, if one more 2 comes, it will not find the "chi"
		for ( int i=0; i<(int)cardList.size()-1; i++ )
		{
			if ( curCard->getCardType() <= Card::CARD_BING )
			{
				if( curCard->getCardType() == cardList[i]->getCardType()
					&& curCard->getCardType() == cardList[i+1]->getCardType() )
				{
					if ( curCard->getCardNumber() == cardList[i]->getCardNumber()-1
						&& curCard->getCardNumber() == cardList[i+1]->getCardNumber()-2 
						||
						 curCard->getCardNumber() == cardList[i]->getCardNumber()+1
						&& curCard->getCardNumber() == cardList[i+1]->getCardNumber()-1
						||
						 curCard->getCardNumber() == cardList[i]->getCardNumber()+1
						&& curCard->getCardNumber() == cardList[i+1]->getCardNumber()+2 )
					{
						singlePushBack(actions, Rule::CHI);
						outCards.push_back(cardList[i]);
						outCards.push_back(cardList[i+1]);
					}
				}
			}
		}
	}

	
	//check hu
	std::vector<Card*> copyVec;
	for ( int i=0; i<(int)cardList.size(); i++ )
		copyVec.push_back(cardList[i]);
	insertCards(curCard, copyVec, 1);

	if ( copyVec.size() == MAX_START_NUM+1 )
	{
		//check 7 dui
		int i = 0;
		for ( i=0; i<=MAX_START_NUM; i+=2 )
		{
			if ( ! (*copyVec[i] == *copyVec[i+1]) )
				break;
		}
		if ( i >= MAX_START_NUM-1 )
		{
			singlePushBack(actions, Rule::HU);
		}
	}

	const int totalCardNum = Card::CARD_CHUNXIAQIUDONG + 3;
	int cards[totalCardNum] = {0};
	convertVecToArray(copyVec, cards, totalCardNum);

	if ( checkHu(cards) )
	{
		singlePushBack(actions, Rule::HU);
	}
}
	