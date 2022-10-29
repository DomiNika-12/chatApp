#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int iSocketFd;
struct sockaddr_in serv;
socklen_t iServSize;
struct sockaddr_in from;
socklen_t iFromSize;

int InitializeParameters()
{
    int iError = 0;

    iSocketFd = 0;
    iServSize = sizeof(serv);
    bzero((char *) &serv, iServSize);
    iFromSize = sizeof(from);
    bzero((char *) &from, iFromSize);

    return iError;
}

int CreateConnection()
{
    int iError = 0;
    int iPort = 0;

    // Create socket: IPv4 domain, UDP, default protocol
    iSocketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (iSocketFd == -1)
    {
        printf("Error while creating socket\n");
        iError = iSocketFd;
        return iError;
    }

    iPort = 12020;
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(iPort);

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
    printf("            %d (hostorder)\n", iPort);
    printf("Domain:     AF_INET\n");
    printf("Protocol:   UDP\n\n");
    return iError;
}

int RecieveMsg()
{
    int iMsgSize = 0;
    char* pcMsg = nullptr;
    int iRecvBytes = 0;
    char* pcIP = nullptr;

    pcIP = (char*) malloc(15);
    iRecvBytes =recvfrom(iSocketFd, &iMsgSize, 4, 0, (struct sockaddr *) &from, &iFromSize);
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

int SendACK()
{
    int iError = 0;

    char* new_str = (char*)"Server: Message received.";
    uint32_t total_msg_size = strlen(new_str);
    int tot_size = htonl(total_msg_size);

    sendto(iSocketFd, &tot_size, sizeof(new_str), 0, (struct sockaddr *)&from, iFromSize);
    printf("Sending ACK length <%d> (network byte order) <%d> (host byte order)\n", htonl(total_msg_size), total_msg_size);

    sendto(iSocketFd, new_str, total_msg_size, 0, (struct sockaddr  *)&from, iFromSize);

    return iError;
}

int main()
{
    int iError = 0;

    iError = InitializeParameters();
    if (iError != 0)
    {
        exit(EXIT_FAILURE);
    }

    iError = CreateConnection();
    if (iError != 0)
    {
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        iError = RecieveMsg();
        if (iError < 0)
        {
            exit(EXIT_FAILURE);
        }
        iError = SendACK();
        if (iError < 0)
        {
            exit(EXIT_FAILURE);
        }
        printf("------------\n");
    }
}
