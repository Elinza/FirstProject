#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <thread>
#include <pthread.h>

#define MAXLINE 1024
#define SUM 1000
#define PORT 8888

using namespace std;

const char* IPaddress = "127.0.0.1";
int max(int a, int b)
{
    return a > b ? a : b;
}

void StrCli()
{
    int maxfdp1;
    fd_set rset;
    char buf[MAXLINE];
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM,0);
    memset(&servaddr, 0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(IPaddress);

    inet_pton(AF_INET, IPaddress, &servaddr.sin_addr);
    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    strcpy(buf, "ping\n");
    send(sockfd, buf, strlen(buf)+1,0);
    printf("client:%s",buf);

    FD_ZERO(&rset);

    while (1) {
        maxfdp1 = max(FD_SETSIZE,sockfd) + 1;
        FD_SET(sockfd, &rset);
        select(maxfdp1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)) {
            recv(sockfd, buf, sizeof(buf), 0);
            printf("server:%s",buf);
            sleep(10);
            return ;
        }
    }
}

int main()
{
    time_t start, end;
    start = time(NULL);
    int i;
    thread* t[SUM];
    for (i = 0; i < SUM;i++) {
         t[i] = new thread(StrCli);
    }
    for (i = 0; i < SUM;i++) {
        if(t[i] != NULL) {
          t[i]->join();
          delete t[i];
          t[i] = NULL;
       }
    }
    end = time(NULL);
    printf("time=%ld\n", end - start);
    return 0;
}
