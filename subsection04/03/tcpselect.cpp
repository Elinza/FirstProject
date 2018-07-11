#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <thread>

#define PORT 9999
#define LISTEN_MAX 1000
#define MAXLINE 1024
using namespace std;

const char *ipAddreass = "127.0.0.1";



void Server()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int client[LISTEN_MAX];
    int sockfd;
    int clientId = 0;
    int i = 0;
    int nready;
    int maxfd;
    int conn;
    int maxi;
    ssize_t n;
    char buf[MAXLINE];
    fd_set rset, allset;
    socklen_t clilen;
    struct sockaddr_in serAddr,cliaddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serAddr.sin_port = htons(PORT);
    if (bind(listenfd, (struct sockaddr*)&serAddr, sizeof(serAddr)) == -1) {
        printf("ser::bind failed,err=%s\n",strerror(errno));
        return ;
    }
    printf("ser::listen %d PORT\n", PORT);

    if (listen(listenfd, LISTEN_MAX) == -1) {
        printf("ser::listen failed\n");
        return ;
    }
    maxfd = listenfd;
    maxi = -1;

    for (i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    while (1) {
        rset = allset;
        nready = select(maxfd + 1, &rset, NULL, NULL , NULL);
        if (FD_ISSET(listenfd, &rset)) {
            clilen = sizeof(cliaddr);
            conn = accept(listenfd, (struct sockaddr*)&cliaddr,
                    &clilen);
            printf("ser::new client:%s,port:%d\n", inet_ntop(AF_INET,
              &cliaddr.sin_addr, buf, sizeof(buf)),ntohs(cliaddr.sin_port));
            for (i = 0; i < FD_SETSIZE; i++)
                if (client[i] < 0) {
                    client[i] = conn;
                    break;
                }
            if (i == FD_SETSIZE) {
                printf("ser::too many clients\n");
                return ;
            }
            FD_SET(conn, &allset);
            if (conn > maxfd)
                maxfd = conn;
            if (i > maxi)
                maxi = i;
            if (--nready <= 0)
                continue;
        }
        for (i = 0; i <= maxi; i++) {
            if ((sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset)) {
                if ((n = recv(sockfd, buf, sizeof(buf),0)) > 0) {
                    printf("ser::client:%s", buf);
                    strcpy(buf, "pong\n");
                    send(sockfd, buf, sizeof(buf),0);
                    printf("ser::server:%s", buf);
                }
                else {
                    clientId++;
                    printf("ser::client %d finished\n", clientId);
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                }
                if (--nready <= 0)
                    break;
            }
        }
    }
}



void Client()
{
    time_t start[LISTEN_MAX], end[LISTEN_MAX];
    int i = 0;
    int maxfd = 0;
    int maxi;
    int nready = 1;
    int sockfd[LISTEN_MAX];
    fd_set rset, allset;
    char buf[MAXLINE];
    struct sockaddr_in servaddr;

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(ipAddreass);

    inet_pton(AF_INET, ipAddreass, &servaddr.sin_addr);
    FD_ZERO(&allset);

    while (i != LISTEN_MAX) {
        sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (connect(sockfd[i], (struct sockaddr*)&servaddr,
                    sizeof(servaddr)) < 0)
            sockfd[i] = -1;
        start[i] = time(NULL);
        strcpy(buf, "ping\n");
        send(sockfd[i], buf, strlen(buf)+1, 0);
        printf("cli::client send:%s", buf);
        maxfd = (maxfd > sockfd[i] ? maxfd : sockfd[i]) + 1;
        FD_SET(sockfd[i], &allset);
        i++;
    }
    FD_ZERO(&rset);
    maxi = -1;

    while (nready) {
        rset = allset;
        nready = select(maxfd, &rset, NULL, NULL, NULL);
        if (nready <= 0) {
           printf("cli::select false\n err=%s",strerror(errno));
        }
        for (i = 0; i <= LISTEN_MAX; i++) {
            end[i] = time(NULL);
            if ((end[i] - start[i]) >= 10) {
                close(sockfd[i]);
                FD_CLR(sockfd[i], &allset);
            }
            if (sockfd[i] < 0)
                continue;
            if (FD_ISSET(sockfd[i], &rset)) {
                if(recv(sockfd[i], buf, sizeof(buf), 0) > 0) {
                    printf("cli::server:%s", buf);
                }
                else {
                    printf("cli::server send failed\n");
                    close(sockfd[i]);
                    FD_CLR(sockfd[i], &allset);
                }
            }
        }
    }
}



int main()
{
    time_t start, end;
    start = time(NULL);
    thread t1(Server);
    thread t2(Client);

    t1.join();
    t2.join();
    end = time(NULL);
    printf("\ntime:%ld\n", end - start);
    return 0;
}
