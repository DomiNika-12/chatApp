#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cerrno>
#include "types.h"

#ifndef CHATAPP_CLIENT_H
#define CHATAPP_CLIENT_H

#define PORT 49153

class Client {
private:
    struct sockaddr_in serv;
    socklen_t iServSize;
public:
    int iSocketFd;
    Client();
    int CreateConnection();
    int readUserInput(char** buffer);
    int SendMsg(char** pcMsg, int iMsgSize);
    int ReadMsg(char** pcMsg, int* piMsgSize);
};

#endif //CHATAPP_CLIENT_H
