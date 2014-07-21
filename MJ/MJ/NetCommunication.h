#pragma once

#include "communication.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"
#include <ace/Synch.h>
#include <ace/Synch_T.h>
#include <ace/Method_Request.h>
#include <ace/Thread_Manager.h>
#include <string>

class NetCommunication :
	public Communication
{
public:
	NetCommunication(void);
	~NetCommunication(void);
public:
    NetCommunication(bool bServer, int port, char* hostname = 0);
public:
    void init();
    virtual bool send(const CommunicateObj& commObj);
    virtual bool recv(CommunicateObj& commObj);
    virtual bool encode(const CommunicateObj& commObj, char*& target);
    virtual bool decode(const char* src, CommunicateObj& commObj);

public:
	static void* recHandler(void* args)throw();
    
private:
    void serverInit();
    void clientInit();
    bool serverAcceptConnection();
    bool clientRequireConnection();
    
    
//can be shared by server and client
private:
    const static int m_cnErrorRetryTime = 3;
    bool m_bServer;
    int m_nPort;
    bool m_hasInited;
    
//for server
private:

    ACE_INET_Addr m_serverAddr;
	ACE_SOCK_Stream m_serverStream;
	ACE_INET_Addr m_clientAddr;
    ACE_SOCK_Acceptor m_peerAcceptor;
    
//for client
private:
    std::string m_hostName;  
    ACE_SOCK_Stream m_clientStream;
    ACE_INET_Addr m_remoteAddr;
    ACE_SOCK_Connector m_clientConnector;  
};

