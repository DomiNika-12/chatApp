#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "server.h"

int main()
{
    int iError = 0;
    auto* serv = new Server();

    iError = serv->CreateConnection();
    if (iError != 0)
    {
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        iError = serv->ReceiveMsg();
        if (iError < 0)
        {
            exit(EXIT_FAILURE);
        }
        iError = serv->SendACK();
        if (iError < 0)
        {
            exit(EXIT_FAILURE);
        }
        printf("------------\n");
    }
}
