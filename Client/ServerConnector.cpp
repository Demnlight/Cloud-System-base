#include "ServerConnector.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment( lib, "ws2_32.lib" )

void C_Connector::Instance()
{
	WSADATA WSAData;
	int32_t iResult = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (iResult == INVALID_SOCKET)
		return;

	m_iSocket = (int)(socket(AF_INET, SOCK_STREAM, 0));
	if ((SOCKET)(m_iSocket) == INVALID_SOCKET)
		return;

	sockaddr_in pSocketAddress;
	pSocketAddress.sin_family = AF_INET;
	pSocketAddress.sin_port = htons(14312);
	inet_pton(AF_INET, ("127.0.0.1"), &pSocketAddress.sin_addr);

	static int m_iIteration = 0;
	while (!m_bIsConnected)
	{
		m_iIteration++;

		if (m_iIteration > 5)
		{
			LOG_DEBUG("Server Connection Failed | Last error: Timed Out");
			m_bIsConnected = false;

			if (MessageBox(NULL, "Failed to connect \nTry Later", "Server Connection Failed", MB_ICONERROR))
				ExitProcess(1);

			break;
		}

		int32_t iConnectionResult = connect((SOCKET)(m_iSocket), (sockaddr*)(&pSocketAddress), sizeof(sockaddr_in));
		if (iConnectionResult == SOCKET_ERROR)
		{
			Sleep(1000);
			LOG_DEBUG("Server Connection Failed | Last error: %d", iConnectionResult);
			continue;
		}

		m_bIsConnected = true;
	}
}

std::string C_Connector::SendAndReceiveMessage(std::string m_strMessage)
{
	if (m_strMessage.size() <= 0)
		return m_arrAnswers[SEND_MESSAGE_ANSWER::ANSWER_FAILED_SIZE];

	LOG_DEBUG("Send Message to server | Message: %s", m_strMessage.c_str());

	int32_t iSendedBytes = send((SOCKET)(m_iSocket), m_strMessage.c_str(), strlen(m_strMessage.c_str()), 0);
	if (iSendedBytes == INVALID_SOCKET)
		return m_arrAnswers[SEND_MESSAGE_ANSWER::ANSWER_FAILED_SEND];

	char aRecv[512000];
	ZeroMemory(aRecv, 512000);

	int32_t iReceived = recv((SOCKET)(m_iSocket), aRecv, 512000, 0);
	if (iReceived == INVALID_SOCKET)
		return m_arrAnswers[SEND_MESSAGE_ANSWER::ANSWER_FAILED_RECEIVE];

	std::string m_strServerAnswer = aRecv;

	if (m_strServerAnswer.size() <= 0)
		return m_arrAnswers[SEND_MESSAGE_ANSWER::ANSWER_FAILED_SIZE];

	return m_strServerAnswer;
}