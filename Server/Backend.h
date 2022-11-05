#pragma once
#include <string>
#include <vector>
#include <WinSock2.h>

#include "Logger/Logger.h"
#include "../Client/SDK/Message.hpp"

struct C_Client
{
	std::string m_FullAddress;
	char m_Address[64];

	SOCKET m_Socket;
	sockaddr_in m_SocketAddress;
	int32_t m_iClientIndex;
};

class C_Data
{
public:
	SOCKET m_pServerSocket;
	sockaddr_in m_pSocketAddress;

	std::vector<C_Client> m_arrClients;
};

class C_Tools
{
public:
	static void HandleClients();
	static void ReceiveClients();

	virtual void KickClient(C_Client& Client);
	virtual bool SendNetMessage(C_Client& Client, C_Message jMessage)
	{
		std::string strDump = jMessage.GetJSON().dump(4);
		int32_t iLength = strDump.length();

		LOG_DEBUG("Send message to %s | Message %s", Client.m_FullAddress, jMessage.DumpMsg());
		return send(Client.m_Socket, strDump.c_str(), iLength, 0) > 0;

	}
	virtual bool SendNetMessage(C_Client& Client, std::string jMessage)
	{
		std::string strDump = jMessage;
		int32_t iLength = strDump.length();

		LOG_DEBUG("Send message to %s | Message %s", Client.m_FullAddress.c_str(), jMessage.c_str());

		return send(Client.m_Socket, strDump.c_str(), iLength, 0) > 0;
	}
private:

};

class C_Events
{
public:
	virtual void AppVersion(C_Client& Client, C_Message& jMessage);

private:

};

inline C_Events* g_Events = new C_Events();
inline C_Data* g_Data = new C_Data();
inline C_Tools* g_Tools = new C_Tools();