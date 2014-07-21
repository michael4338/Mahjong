#pragma once

#include "CommunicateObj.h"

class Communication
{
public:
	Communication(void);
	virtual ~Communication(void);

public:
    virtual void init() = 0;
    virtual bool send(const CommunicateObj& commObj) = 0;
    virtual bool recv(CommunicateObj& commObj) = 0;
    virtual bool encode(const CommunicateObj& commObj, char*& target) = 0;
    virtual bool decode(const char* src, CommunicateObj& commObj) = 0;
};
