#pragma once
#include <string>

#include "SDK/Json.hpp"
#include "SDK/Message.hpp"
#include "Logger/Logger.h"

const int m_iAnswersSize = 4;

enum SEND_MESSAGE_ANSWER : int
{
	ANSWER_OK,
	ANSWER_FAILED_SEND,
	ANSWER_FAILED_SIZE,
	ANSWER_FAILED_RECEIVE,

};

class C_Connector
{
public:
	virtual void Instance();
	virtual std::string SendAndReceiveMessage(std::string jMessage);
	virtual int GetSocket() { return this->m_iSocket; };
	virtual int IsConnected() { return this->m_bIsConnected; };
	virtual void SetSocket(int m_iSocket) { this->m_iSocket = m_iSocket; };
	virtual void SetConnection(bool m_bIsConnected) { this->m_bIsConnected = m_bIsConnected; };
	virtual bool IsMessageValid(std::string m_strMessage)
	{ 
		for (int i = 1; i < m_iAnswersSize - 1; i++)
		{
			if (m_strMessage.find(this->m_arrAnswers[i]) != std::string::npos)
				return false;

			return true;
		}
		return true; 
	};

private:
	int m_iSocket = 0;
	bool m_bIsConnected = false;

	std::string m_arrAnswers[m_iAnswersSize] =
	{
		"Answer:OK",
		"Answer:SizeCheckFailed",
		"Answer:SendMessageFailed",
		"Answer:ReceiveMessageFailed"
	};
};

inline C_Connector* g_Connector = new C_Connector();