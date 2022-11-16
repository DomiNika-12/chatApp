#include "server.h"

Server::Server()
{
    iSocketFd = 0;
    iServSize = sizeof(serv);
    bzero((char *) &serv, iServSize);
    iFromSize = sizeof(from);
    bzero((char *) &from, iFromSize);
    //clientData = (ClientTable*) malloc(sizeof(ClientTable) * std::thread::hardware_concurrency());
}

int Server::CreateConnection()
{
    int iError = 0;

    // Create socket: IPv4 domain, UDP, default protocol
    iSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (iSocketFd == -1)
    {
        printf("Error while creating socket\n");
        iError = iSocketFd;
        return iError;
    }

    int opt = 1;
    setsockopt(iSocketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(PORT);

    fflush(stdout);
    // Binding socket
    iServSize = sizeof(serv);
    iError = ::bind(iSocketFd, (struct sockaddr *) &serv, iServSize);
    if (iError < 0)
    {
        printf("Failed to bind the socket\n");
        close(iSocketFd);
        return iError;
    }

    iError = listen(iSocketFd, 5);
    if (iError < 0)
    {
        printf("Failed to listen on the socket\n");
        close(iSocketFd);
        return iError;
    }

    printf("Server running\n");
    printf("Port:       %d (network byte order)\n", serv.sin_port);
    printf("            %d (hostorder)\n", PORT);
    printf("Domain:     AF_INET\n");
    printf("Protocol:   TCP\n\n");
    return iError;
}

int Server::ReadMsg(int iConSocketFd, char** ppcMsg, int* piMsgSize)
{
    int iError = 0;

    iError = read(iConSocketFd, piMsgSize, 4);
    if (iError < 0)
    {
        printf("Read failed\n");
    }

    *ppcMsg = (char *) malloc(sizeof(char) * (ntohl(*piMsgSize) + 1));
    read(iConSocketFd, *ppcMsg, *piMsgSize);
    if (iError < 0)
    {
        printf("Read failed\n");
    }
}

int Server::SendMsg(int iConSocketFd, char** ppcMsg, int iMsgSize)
{
    int iError = 0;

    iError = send(iConSocketFd, &iMsgSize, 4, 0);
    if (iError < 0)
    {
        printf("Size of the message not sent, errno (%d), %s\n", iError, errno, strerror(errno));
        return iError;
    }
    printf("Size of the message sent: %d\n", iMsgSize);

    iError = send(iConSocketFd, *ppcMsg, iMsgSize, 0);
    if (iError < 0)
    {
        printf("Message content not sent, errno (%d), %s\n", errno, strerror(errno));
        return iError;
    }
    printf("Message sent: %s\n", (char*) *ppcMsg);

    return iError;
}

int Server::GetSocket() {
    return iSocketFd;
}

int Server::GetClientID(int iConSocketFd, IDHeader* pHeader) {
    int iError = 0;
    ClientTable data{};

    iError = read(iConSocketFd, pHeader, sizeof(*pHeader));
    if (iError < 0)
    {
        printf("Read of clientID failed\n");
    }

    data.cClientID = pHeader->clientID;
    data.cSelfID = pHeader->selfID;
    data.iSocketFD = iConSocketFd;
    clientData.push_back(data);

    return 0;
}

