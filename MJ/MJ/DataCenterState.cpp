#include "StdAfx.h"
#include "DataCenterState.h"
#include "DataCenterImpl.h"
#include "Rule.h"

DataCenterState* DataCenterStartState::theInstance = 0;
ACE_Thread_Mutex DataCenterStartState::instanceMutex;

DataCenterState::DataCenterState(void)
{
}

DataCenterState::~DataCenterState(void)
{
}


DataCenterState* const DataCenterStartState::getInstance()
{
	if(theInstance == 0)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(instanceMutex);
		if(guard.locked() == 0) 
		    return 0;
		if(theInstance == 0)
		{
			theInstance = new DataCenterStartState();
		}
	}
	return theInstance;
}

void 
DataCenterStartState::
processRequest(int playerID, const CommunicateObj& commObj, DataCenterImpl* dcImpl)
{
	if ( commObj.m_msgFlag != CommunicateObj::MSG_TOKEN )
		return;

	dcImpl->gameStart();
	dcImpl->changeState(DataCenterInitialState::getInstance());
}



DataCenterState* DataCenterInitialState::theInstance = 0;
ACE_Thread_Mutex DataCenterInitialState::instanceMutex;

DataCenterState* const DataCenterInitialState::getInstance()
{
	if(theInstance == 0)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(instanceMutex);
		if(guard.locked() == 0) 
		    return 0;
		if(theInstance == 0)
		{
			theInstance = new DataCenterInitialState();
		}
	}
	return theInstance;
}

void 
DataCenterInitialState::
processRequest(int playerID, const CommunicateObj& commObj, DataCenterImpl* dcImpl)
{
	if ( commObj.m_msgFlag != CommunicateObj::MSG_INITCARDS )
		return;
		
	dcImpl->dispatchInitialCards();
	dcImpl->changeState(DataCenterWaitInCardState::getInstance());
}



DataCenterState* DataCenterWaitInCardState::theInstance = 0;
ACE_Thread_Mutex DataCenterWaitInCardState::instanceMutex;

DataCenterState* const DataCenterWaitInCardState::getInstance()
{
	if(theInstance == 0)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(instanceMutex);
		if(guard.locked() == 0) 
		    return 0;
		if(theInstance == 0)
		{
			theInstance = new DataCenterWaitInCardState();
		}
	}
	return theInstance;
}

void 
DataCenterWaitInCardState::
processRequest(int playerID, const CommunicateObj& commObj, DataCenterImpl* dcImpl)
{
	if ( commObj.m_msgFlag != CommunicateObj::MSG_OUTCARD )
		return;
		
	dcImpl->dispatchCard(playerID);
	dcImpl->changeState(DataCenterWaitActionState::getInstance());
}


DataCenterState* DataCenterWaitActionState::theInstance = 0;
ACE_Thread_Mutex DataCenterWaitActionState::instanceMutex;

DataCenterState* const DataCenterWaitActionState::getInstance()
{
	if(theInstance == 0)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(instanceMutex);
		if(guard.locked() == 0) 
		    return 0;
		if(theInstance == 0)
		{
			theInstance = new DataCenterWaitActionState();
		}
	}
	return theInstance;
}

void 
DataCenterWaitActionState::
processRequest(int playerID, const CommunicateObj& commObj, DataCenterImpl* dcImpl)
{
	if ( commObj.m_msgFlag != CommunicateObj::MSG_INITCARDS )
		return;
		
	Rule::ACTION action = commObj.m_action;
	bool result = false;

	if ( action == Rule::NOTHING )
		dcImpl->dispatchCard(playerID);
	else 
		result = dcImpl->handleAction(playerID, action);
	
	if ( ! result )
	{
		dcImpl->dispatchCard(playerID);
		dcImpl->changeState(DataCenterWaitInCardState::getInstance());
	}
	else
	{
		if ( action == Rule::HU )
		{ 
			dcImpl->changeState(DataCenterStartState::getInstance());
		}
		else
		{
			dcImpl->changeState(DataCenterWaitInCardState::getInstance());
		}
	}
}

