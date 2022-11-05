#include <iostream>
#include <Windows.h>

#include "ServerConnector.h"
#include "Logger/Logger.h"

int main()
{
    g_Connector->Instance();

    std::string m_strVersion = g_Connector->SendAndReceiveMessage("Server:Data:Version");

    if (!g_Connector->IsMessageValid(m_strVersion))
        EXIT_APP(-1);

    LOG_DEBUG("Server:Data:Version::%s", m_strVersion.c_str());

    if (m_strVersion == "1.0")
    {
        LOG_DEBUG("App version is correct");
        while (true)
        {
            std::string m_strClientIsActive = g_Connector->SendAndReceiveMessage("Client:Data:Active");

            if (!g_Connector->IsMessageValid(m_strClientIsActive))
            {
                LOG_ERROR("Message Invalid | Message: %s", m_strClientIsActive.c_str());
                EXIT_APP(-1);
            }

            Sleep(500);
        }
    }
    else
    {
        LOG_CRIT("App version is incorrect, please update");
        EXIT_APP(-1);
    }

    EXIT_APP(0);
}