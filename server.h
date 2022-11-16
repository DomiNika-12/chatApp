#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <thread>
#include <vector>

#ifndef CHATAPP_SERVER_H
#define CHATAPP_SERVER_H

#define PORT 49153
#define ACKMSG "Server: Message received."

using namespace std;

struct ClientTable {
    char cClientID;
    char cSelfID;
    int iSocketFD;
};

struct IDHeader {
    char selfID;
    char clientID;
};
class Server {
private:
    int iSocketFd;
    struct sockaddr_in serv;
    socklen_t iServSize;
    struct sockaddr_in from;
    socklen_t iFromSize;
public:
    Server();
    vector<ClientTable> clientData{};
    int CreateConnection();
    int GetClientID(int iConSocketFd, IDHeader* pHeader);
    int SendMsg(int iConSocketFd, char** ppcMsg, int iMsgSize);
    int ReadMsg(int iConSocketFd, char** ppcMsg, int* piMsgSize);
    int GetSocket();
};

#endif //CHATAPP_SERVER_H
