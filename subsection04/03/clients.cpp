#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>

#define PORT 8800
#define BUF_SIZE 10
#define RUN_TIME 1000


const char *SERVER_IP = "127.0.0.1";
void Thread(int sockCli, int conn)
{
    char sendBuf[BUF_SIZE] = "ping\n";
    char recvBuf[BUF_SIZE];
        send(sockCli, sendBuf, strlen(sendBuf) + 1, 0);
        printf("send message to server:%s", sendBuf);
        if ((recv(sockCli, recvBuf, sizeof(recvBuf), 0) ) > 0) {
            printf("receive from server:%s", recvBuf);
        }
        else {
            printf("receive wrong!!\n");
        }
        close(sockCli);
        memset(sendBuf, 0, sizeof(sendBuf));
}

int main()
{
   // char *SERVER_IP = (char*)malloc((strlen("127.0.0.1")+1));
   // strcpy(SERVER_IP, "127.0.0.1");
    int sockCli = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(PORT);
    serAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    printf("link %s:%d\n", SERVER_IP, PORT);

    printf("link successfully\n");
    char sendBuf[BUF_SIZE] = "ping\n";
    char recvBuf[BUF_SIZE];
        if(connect(sockCli, (struct sockaddr*)&serAddr,
            sizeof(serAddr)) < 0) {
            printf("connect wrong!!\n");
        }
        send(sockCli, sendBuf, strlen(sendBuf) + 1, 0);
        printf("send message to server:%s", sendBuf);
        if ((recv(sockCli, recvBuf, sizeof(recvBuf), 0) ) > 0) {
            printf("receive from server:%s", recvBuf);
        }
        else {
            printf("receive wrong!!\n");
        }
        close(sockCli);
        memset(sendBuf, 0, sizeof(sendBuf));
        memset(recvBuf, 0, sizeof(recvBuf));
    return 0;
}
