#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#ifndef CHATAPP_SERVER_H
#define CHATAPP_SERVER_H

#define PORT 12020
#define ACKMSG "Server: Message received."

class Server {
private:
    int iSocketFd;
    struct sockaddr_in serv;
    socklen_t iServSize;
    struct sockaddr_in from;
    socklen_t iFromSize;
public:
    Server();
    int CreateConnection();
    int ReceiveMsg();
    int SendACK();
};

#endif //CHATAPP_SERVER_H
