//
// Created by Dominika Bobik on 10/26/22.
//

#include "server.h"

Server::Server()
{
    iSocketFd = 0;
    iServSize = sizeof(serv);
    bzero((char *) &serv, iServSize);
    iFromSize = sizeof(from);
    bzero((char *) &from, iFromSize);
}

int Server::CreateConnection()
{
    int iError = 0;

    // Create socket: IPv4 domain, UDP, default protocol
    iSocketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (iSocketFd == -1)
    {
        printf("Error while creating socket\n");
        iError = iSocketFd;
        return iError;
    }

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(PORT);

    fflush(stdout);
    // Binding socket
    iError = bind(iSocketFd, (struct sockaddr *) &serv, iServSize);
    if (iError < 0)
    {
        printf("Failed to bind the socket\n");
        close(iSocketFd);
        return iError;
    }

    iServSize = sizeof(serv);
    getsockname(iSocketFd, (struct sockaddr *) &serv, (socklen_t *)&iServSize);

    printf("Server running\n");
    printf("Port:       %d (network byte order)\n", serv.sin_port);
    printf("            %d (hostorder)\n", PORT);
    printf("Domain:     AF_INET\n");
    printf("Protocol:   UDP\n\n");
    return iError;
}

int Server::ReceiveMsg()
{
    int iMsgSize = 0;
    char* pcMsg = nullptr;
    int iRecvBytes = 0;
    char* pcIP = nullptr;

    pcIP = (char*) malloc(15);
    iRecvBytes = recvfrom(iSocketFd, &iMsgSize, 4, 0, (struct sockaddr *) &from, &iFromSize);
    printf("Server got request\n");
    inet_ntop(AF_INET, &(from.sin_addr), pcIP,15);
    printf("IP: %s\n", pcIP);
    printf("Number of bytes: %d\n Request content:\n%d\n", iRecvBytes, ntohl(iMsgSize));

    pcMsg = (char *) malloc(sizeof(char) * (ntohl(iMsgSize) + 1));
    iRecvBytes = recvfrom(iSocketFd, (char *) pcMsg, iMsgSize, 0, (struct sockaddr *) &from, &iFromSize);
    printf("Number of bytes: %d\n Message content:\n%s\n", iRecvBytes, pcMsg);

    free(pcIP);
    free(pcMsg);
}

int Server::SendACK()
{
    int iError = 0;
    char* pcACKMsg = nullptr;
    int iACKMsgSize = 0;
    int iACKMsgSizeNBO = 0;

    pcACKMsg = (char*) ACKMSG;
    iACKMsgSize = strlen(pcACKMsg);
    iACKMsgSizeNBO = htonl(iACKMsgSize);

    iError = sendto(iSocketFd, &iACKMsgSizeNBO, sizeof(pcACKMsg), 0, (struct sockaddr *)&from, iFromSize);
    if (iError < 0)
    {
        printf("Size message not sent, error (%d), errno (%d)\n", iError, errno);
        return iError;
    }
    printf("Sending ACK length <%d> (network byte order) <%d> (host byte order)\n", htonl(iACKMsgSize), iACKMsgSize);

    iError = sendto(iSocketFd, pcACKMsg, iACKMsgSize, 0, (struct sockaddr  *)&from, iFromSize);
    if (iError < 0)
    {
        printf("ACK message not sent, error (%d), errno (%d)\n", iError, errno);
        return iError;
    }

    return iError;
}