#pragma once

#include "Json.hpp"

enum Results
{
	RESULT_OK = 0,
	RESULT_FAIL = 1
};

enum MSG_TYPE
{
	MSG_HELLO = 0,
	MSG_REGISTER = 1,
	MSG_LOGIN = 2,
	MSG_PUSHLIST = 3,
	MSG_SAVELIST = 4,
	MSG_LOGINTOKEN = 5
};

class C_Message
{
public:
	C_Message(std::string strText)
	{
		jMessage[("m_strMessage")] = strText;
	}

	int32_t GetMessageType( )
	{
		return jMessage[( "Data" ) ][( "Type" ) ].get< int32_t >( );
	}

	nlohmann::json& GetJSON( )
	{
		return jMessage;
	}

	bool Accept( char* cData )
	{
		if ( !jMessage.accept( cData ) )
			return false;

		jMessage = jMessage.parse( cData );
		return true;
	} 

	const char* DumpMsg( )
	{
		return jMessage.dump( 4 ).c_str( );
	}

	int32_t GetMsgLength( )
	{
		return strlen( DumpMsg( ) );
	}

	std::string GetMsgBlank(std::string cBlank)
	{
		return jMessage[cBlank].get< std::string >();
	}

private:
	nlohmann::json jMessage;
};