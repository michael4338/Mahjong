#include "StdAfx.h"
#include "NetCommunication.h"
#include "Rule.h"

const int MAX_LEN = 1024;

NetCommunication::NetCommunication(bool bServer, 
    int port,
    char* hostName):m_bServer(bServer),
    m_nPort(port),
    m_hasInited(false),
    m_hostName(hostName==0?"":hostName),
    m_serverAddr(port),
    m_peerAcceptor(m_serverAddr),
    m_remoteAddr(port, hostName)
{
}

NetCommunication::~NetCommunication()
{
	m_serverStream.close();
    m_clientStream.close();
}

void NetCommunication::init()
{
    if(m_hasInited)
    {
        AfxMessageBox(LPCTSTR("the network communication can only be inited once"));
        return;
    }
    
    if(m_bServer)
        serverInit();
    else
        clientInit();
        
    m_hasInited = true;
    return ;
}

void NetCommunication::serverInit()
{
    if(!serverAcceptConnection())
    {
        AfxMessageBox(LPCTSTR("server init failure: can not finish accepting operation"));
        return;
    }   
}


void NetCommunication:: clientInit()
{
    if(!clientRequireConnection())
    {
        AfxMessageBox(LPCTSTR("client init failure: can not finish connecting operation"));
        return;
    }
}

bool NetCommunication::serverAcceptConnection()
{
    if(m_peerAcceptor.get_local_addr(m_serverAddr) == -1)
        return false;
          
	while(true)
	{
		ACE_Time_Value timeout(ACE_DEFAULT_TIMEOUT);
		if(m_peerAcceptor.accept(m_serverStream, &m_clientAddr, &timeout) == -1)
			continue;
		else
			break;
	}
    return true;
}

bool NetCommunication::clientRequireConnection()
{
    if (m_clientConnector.connect(m_clientStream, m_remoteAddr) == -1)
        return false;
    return true;
}

bool NetCommunication::send(const CommunicateObj& commObj)
{
	char* buf = new char[MAX_LEN];
	memset(buf, 0, MAX_LEN);

	encode(commObj, buf);
        
	if((m_bServer?m_serverStream:m_clientStream).send_n(buf, strlen(buf)+1, 0) == -1)
    {
        delete []buf;
        return false;
    }

    delete []buf;
	return true;
}

bool NetCommunication::recv(CommunicateObj& commObj)
{
	char* buf = new char[MAX_LEN];
	memset(buf, 0, MAX_LEN);

	int byteCount=0;
	if((byteCount=(m_bServer?m_serverStream:m_clientStream).recv_n(buf, MAX_LEN, 0)) == -1)
	{
		delete []buf;
        return false;
	}
	else
		buf[byteCount] = 0;

	decode(buf, commObj);

	delete []buf;
	return true;
}

bool NetCommunication::encode(const CommunicateObj& commObj, char*& target)
{
	CommunicateObj* ptr = const_cast<CommunicateObj*>(&commObj);
	return ptr->serialize(target);
}

bool NetCommunication::decode(const char* src, CommunicateObj& commObj) 
{
	return commObj.deserialize(src);
}


void* NetCommunication::recHandler(void *args) throw()
{
	if(args == 0) 
		return 0;

	NetCommunication* thisObject = static_cast<NetCommunication*> (args);

	while(true)
	{
		CommunicateObj commObj;
		if(thisObject->recv(commObj))
		{
		}
	}

	return 0;
}


	