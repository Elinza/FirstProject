#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define MAXLINE 1024
#define LISTEN_MAX 1000
#define PORT 9999


const char* IPaddress = "127.0.0.1";
int max(int a, int b)
{
    return a > b ? a : b;
}

void StrCli()
{
    int i = 0;
    int maxfd = 0;
    int conn;
    int maxi;
    int sockfd[LISTEN_MAX];
    socklen_t clilen;
    fd_set rset,allset;
    char buf[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;

    memset(&servaddr, 0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(IPaddress);

    inet_pton(AF_INET, IPaddress, &servaddr.sin_addr);
    FD_ZERO(&allset);
    while (i != LISTEN_MAX) {
        sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (connect(sockfd[i], (struct sockaddr*)&servaddr,
             sizeof(servaddr)) < 0)
            sockfd[i] = -1;
        strcpy(buf, "ping\n");
        send(sockfd[i], buf, strlen(buf)+1,0);
        printf("client send:%s",buf);
        maxfd = max(maxfd,sockfd[i]) + 1;
        FD_SET(sockfd[i], &allset);
        i++;
    }
    FD_ZERO(&rset);
    maxi = -1;

    while (1) {
        rset = allset;
        select(maxfd, &rset, NULL, NULL, NULL);
        for (i = 0; i <= LISTEN_MAX; i++) {
            if (sockfd[i] < 0)
                continue;
            if (FD_ISSET(sockfd[i], &rset)) {
               if (recv(sockfd[i], buf, sizeof(buf), 0) > 0) {
                   printf("server:%s",buf);
               }
               else {
                   printf("server send failed\n");
                   close(sockfd[i]);
                   FD_CLR(sockfd[i], &allset);
               }
            }
        }
    }
}

int main()
{
    StrCli();
    return 0;
}
