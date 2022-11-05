#include <WS2tcpip.h>

#include "Backend.h"
#include "Logger/Logger.h"
#include "../Client/SDK/Message.hpp"

#pragma comment( lib, "ws2_32.lib" )

int main()
{
    WSADATA pWSAData;
    if (WSAStartup(MAKEWORD(2, 2), &pWSAData))
    {
        LOG_CRIT("Unable to initializate the WSA");
        return 1;
    }

    g_Data->m_pServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (!g_Data->m_pServerSocket)
    {
        LOG_CRIT("Unable to create the server socket");
        return 1;
    }

    g_Data->m_pSocketAddress.sin_family = AF_INET;
    g_Data->m_pSocketAddress.sin_port = htons(14312);
    g_Data->m_pSocketAddress.sin_addr.S_un.S_addr = INADDR_ANY;

    int32_t iBindResult = bind(g_Data->m_pServerSocket, (sockaddr*)(&g_Data->m_pSocketAddress), 16);
    if (iBindResult < 0)
    {
        LOG_CRIT("Server cannot binded");
        return 1;
    }

    int32_t iListenResult = listen(g_Data->m_pServerSocket, SOMAXCONN);
    if (iListenResult < 0)
    {
        LOG_CRIT("Cannot listen clients");
        return 1;
    }

    LOG_INFO("Server started up");

    LOG_INFO("Awaiting new clients... ");

    std::thread hReceiveClient(C_Tools::ReceiveClients);
    hReceiveClient.detach();

    std::thread hHandleClients(C_Tools::HandleClients);
    hHandleClients.detach();

    while (true)
    {
        Sleep(200);
    }

    return 0;
}