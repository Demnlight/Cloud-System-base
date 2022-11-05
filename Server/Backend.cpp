#include "Backend.h"
#include <WS2tcpip.h>

#pragma comment( lib, "ws2_32.lib" )

void C_Tools::ReceiveClients()
{
	while (true)
	{
		Sleep(50);

		sockaddr_in pSocketAddress;
		int32_t iClientSize = sizeof(pSocketAddress);

		SOCKET pClientSocket = accept(g_Data->m_pServerSocket, (sockaddr*)(&pSocketAddress), &iClientSize);
		if (!pClientSocket)
		{
			LOG_ERROR("Non accepted");
			continue;
		}

		C_Client Client;
		Client.m_Socket = pClientSocket;
		Client.m_SocketAddress = pSocketAddress;
		Client.m_iClientIndex = g_Data->m_arrClients.size();

		inet_ntop(AF_INET, &Client.m_SocketAddress.sin_addr, Client.m_Address, 64);
		Client.m_FullAddress += Client.m_Address;
		Client.m_FullAddress += ":" + std::to_string(ntohs(Client.m_SocketAddress.sin_port));

		LOG_INFO("New Client received | Client address: %s", Client.m_FullAddress.c_str());

		g_Data->m_arrClients.emplace_back(Client);
	}
}

void C_Tools::HandleClients()
{
	while (true)
	{
		Sleep(10);
		if (g_Data->m_arrClients.empty())
			Sleep(10);

		if (g_Data->m_arrClients.size() > 0)
		{
			for (C_Client& Client : g_Data->m_arrClients)
			{
				char aReceived[512000];
				ZeroMemory(aReceived, 512000);

				int32_t iRecvBytes = recv(Client.m_Socket, aReceived, 512000, 0);
				if (iRecvBytes < 0)
				{
					g_Tools->KickClient(Client);
					continue;
				}

				C_Message jAcceptedMessage(aReceived);
				nlohmann::json m_jClientJSON = jAcceptedMessage.GetJSON();
				std::string m_strClientMessage = jAcceptedMessage.DumpMsg();
				std::string m_strBlankMessage = jAcceptedMessage.GetMsgBlank("m_strMessage");

				if (m_strBlankMessage.find("Server:Data:Version") != std::string::npos)
					g_Events->AppVersion(Client, jAcceptedMessage);
				else if (m_strBlankMessage.find("Client:Data:Active") != std::string::npos)
					Sleep(500);
			}
		}
	}
}

void C_Tools::KickClient(C_Client& Client)
{
	std::string strLogMsg = "";
	strLogMsg += Client.m_FullAddress;
	strLogMsg += " disconnected from the server";
	LOG_INFO(strLogMsg.c_str());
	closesocket(Client.m_Socket);

	g_Data->m_arrClients.erase(g_Data->m_arrClients.begin() + Client.m_iClientIndex);

	for (int32_t iClient = 0; iClient < g_Data->m_arrClients.size(); iClient++)
	{
		if (iClient < Client.m_iClientIndex)
			continue;

		g_Data->m_arrClients[iClient].m_iClientIndex--;
	}
}

void C_Events::AppVersion(C_Client& Client, C_Message& jMessage)
{
	std::string m_strMessage = jMessage.GetMsgBlank("m_strMessage");
	g_Tools->SendNetMessage(Client, "1.0");
}