#include <iostream>
#include <cstdlib>
#include <thread>
#include "server.h"

using namespace std;

Server* serv = new Server();

void ThreadMain(int iConSocketFd)
{
    int iError = 0;
    char* pcMsg = nullptr;
    int iMsgSize = 0;
    int iClientSocketFd = 0;
    IDHeader header{};

    cout << "Thread ID: " << this_thread::get_id() << " running\n";
    cout << "Socket: " << iConSocketFd << endl;

    // Send ID to the user

    iError = serv->GetClientID(iConSocketFd, &header);
    if (iError < 0)
    {
        exit(EXIT_FAILURE);
    }

    while(iClientSocketFd == 0)
    {
        for (int i = 0; i < serv->clientData.size(); i++)
        {
            if (serv->clientData.at(i).cSelfID == header.clientID)
            {
                iClientSocketFd = serv->clientData.at(i).iSocketFD;
            }
        }
    }


    while(1)
    {
        iError = serv->ReadMsg(iConSocketFd, &pcMsg, &iMsgSize);
        if (iError < 0)
        {
            exit(EXIT_FAILURE);
        }
        if (iClientSocketFd)
        iError = serv->SendMsg(iClientSocketFd, &pcMsg, iMsgSize);
        if (iError < 0)
        {
            exit(EXIT_FAILURE);
        }
        free(pcMsg);
    }
}
int main()
{
    int iError = 0;

    iError = serv->CreateConnection();
    if (iError != 0)
    {
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // Accept incoming connection
        auto* connection = (sockaddr_in *)malloc(sizeof(sockaddr_in));
        socklen_t len = 0;
        int iConSocketFd = 0;
        iConSocketFd = accept(serv->GetSocket(), (struct sockaddr *) &connection, &len);
        if (iConSocketFd <= 0)
        {
            exit(EXIT_FAILURE);
        }

        thread t(&ThreadMain, iConSocketFd);
        t.detach();
    }
}
