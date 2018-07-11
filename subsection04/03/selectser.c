#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 1024
#define LISTENLEN 1000
#define PORT 8888



int main()
{
    int clientId;
    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set rset, allset;
    char buf[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, LISTENLEN);

    maxfd = listenfd;
    maxi = -1;
    for (i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    while (1) {
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(listenfd,&rset)) {
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
            printf("new client:%s,port %d\n", inet_ntop(AF_INET,
                   &cliaddr.sin_addr, buf, sizeof(buf)),ntohs(cliaddr.sin_port));
            for (i = 0; i < FD_SETSIZE; i++)
                if(client[i] < 0) {
                    client[i] = connfd;
                    break;
                }
            if (i == FD_SETSIZE) {
                printf("too many clients\n");
                return 0;
            }
            FD_SET(connfd, &allset);
            if (connfd > maxfd)
                maxfd = connfd;
            if (i > maxi)
                maxi = i;
            if (--nready <= 0)
                continue;
        }
        for (i = 0; i <= maxi; i++) {
            if ((sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset)) {
                if ((n = recv(sockfd, buf, sizeof(buf), 0)) > 0) {
                    printf("client:%s", buf);
                    strcpy(buf, "pong\n");
                    send(sockfd, buf, sizeof(buf), 0);
                    printf("server:%s", buf);
                }
                else {
                    clientId++;
                    printf("client%d finished\n",clientId);
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                }
                if (--nready <= 0)
                    break;
            }
        }
    }
    return 0;
}
