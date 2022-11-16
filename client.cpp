#include "client.h"

Client::Client()
{
    iSocketFd = 0;
    iServSize = sizeof(serv);
    bzero((char *) &serv, iServSize);
}

int Client::readUserInput(char** buffer)
{
    char c = 0;
    int iInitialSize = 5;
    int i = 0;

    *buffer = (char*) malloc(sizeof(char) * 5);
    c = fgetc(stdin);

    while (c != 10)
    {
        if (i > iInitialSize)
        {
            *buffer = (char*) realloc(*buffer, iInitialSize * 2);
            iInitialSize = iInitialSize * 2;
        }
        (*buffer)[i] = c;
        i++;
        c = fgetc(stdin);
    }
    if (i > iInitialSize)
    {
        *buffer = (char*) realloc(*buffer, iInitialSize * 2);
        iInitialSize = iInitialSize * 2;
    }
    (*buffer)[i] = '\n';
    i++;
    return i;
}

int Client::CreateConnection()
{
    int iError = 0;
    int iOpt = 0;

    // Create socket: IPv4 domain, TCP, TCP protocol
    iSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (iSocketFd == -1)
    {
        printf("Error while creating socket (%d) %s \n", errno, strerror(errno));
        iError = iSocketFd;
        return iError;
    }

    iOpt = 1;
    setsockopt(iSocketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &iOpt, sizeof(iOpt));

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    //inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr);

    iError = connect(iSocketFd, (struct sockaddr *) &serv, sizeof(serv));
    if (iError < 0)
    {
        printf("Error while connecting to the socket (%d) %s \n", errno, strerror(errno));
        return iError;
    }
    printf("Client running\n");
    printf("Port:       %d (network byte order)\n", serv.sin_port);
    printf("            %d (hostorder)\n", PORT);
    printf("Domain:     AF_INET\n");
    printf("Protocol:   TCP\n\n");

    return iError;
}

int Client::ReadMsg(char** ppcMsg, int* piMsgSize)
{
    int iError = 0;

    iError = read(iSocketFd, piMsgSize, 4);
    if (iError < 0)
    {
        printf("Read failed\n");
    }

    *ppcMsg = (char *) malloc(sizeof(char) * (ntohl(*piMsgSize) + 1));
    iError = read(iSocketFd, *ppcMsg, *piMsgSize);
    if (iError < 0)
    {
        printf("Read failed\n");
    }
}

int Client::SendMsg(char** ppcMsg, int iMsgSize)
{
    int iError = 0;

    iError = send(iSocketFd, &iMsgSize, 4, 0);
    if (iError < 0)
    {
        printf("Size of the message not sent, errno (%d), %s\n", iError, errno, strerror(errno));
        return iError;
    }
    printf("Size of the message sent: %d\n", iMsgSize);

    iError = send(iSocketFd, *ppcMsg, iMsgSize, 0);
    if (iError < 0)
    {
        printf("Message content not sent, errno (%d), %s\n", iError, errno, strerror(errno));
        return iError;
    }
    printf("Message sent: %s\n", *ppcMsg);

    return iError;
}