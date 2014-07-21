#pragma once

#include "CommunicateObj.h"

class DataCenterImpl;

class DataCenterState
{
public:
	DataCenterState(void);
	virtual ~DataCenterState(void);
public:
	virtual void processRequest(int playerID, const CommunicateObj& commObj, DataCenterImpl*) = 0;
	//virtual void processTimeout(int playerID) = 0;
};


class DataCenterStartState: public DataCenterState
{
public:
	static DataCenterState* const getInstance();

public:
	virtual void processRequest(int playerID, const CommunicateObj& commObj, DataCenterImpl*);
	//virtual void processTimeout(int playerID);

private:
	static DataCenterState *theInstance;
	static ACE_Thread_Mutex instanceMutex;
};

class DataCenterInitialState: public DataCenterState
{
public:
	static DataCenterState* const getInstance();

public:
	virtual void processRequest(int playerID, const CommunicateObj& commObj, DataCenterImpl*);
	//virtual void processTimeout(int playerID);

private:
	static DataCenterState *theInstance;
	static ACE_Thread_Mutex instanceMutex;
};

class DataCenterWaitInCardState: public DataCenterState
{
public:
	static DataCenterState* const getInstance();

public:
	virtual void processRequest(int playerID, const CommunicateObj& commObj, DataCenterImpl*);
	//virtual void processTimeout(int playerID);

private:
	static DataCenterState *theInstance;
	static ACE_Thread_Mutex instanceMutex;
};

class DataCenterWaitActionState: public DataCenterState
{
public:
	static DataCenterState* const getInstance();

public:
	virtual void processRequest(int playerID, const CommunicateObj& commObj, DataCenterImpl*);
	//virtual void processTimeout(int playerID);

private:
	static DataCenterState *theInstance;
	static ACE_Thread_Mutex instanceMutex;
};

