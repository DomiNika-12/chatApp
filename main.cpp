#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include "server.h"
#include "client.h"

using namespace std;

Server* serv = new Server();

void ThreadMain(sockaddr_in* pConnection, socklen_t* pLen, int iConSocketFd)
{
    int iError = 0;
    char* pcMsg = nullptr;
    int iMsgSize = 0;
    int iClientSocketFd = 0;
    char cSelfID = '\0';
    IDHeader header{};

    cout << "Thread ID: " << this_thread::get_id() << " running\n";
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
        // accept incoming connections
        sockaddr_in* connection = (sockaddr_in *)malloc(sizeof(connection));
        socklen_t len;
        int iConSocketFd = 0;
        iConSocketFd = accept(serv->GetSocket(), (struct sockaddr *) &connection, &len);
        if (iConSocketFd <= 0)
        {
            exit(EXIT_FAILURE);
        }
        else
        {
            // start a new thread but do not wait for it
            //threads[i++] =
            printf("Socket: %d\n", iConSocketFd);
            thread t3(&ThreadMain, connection, &len, iConSocketFd);
            t3.detach();
        }
    }
}
