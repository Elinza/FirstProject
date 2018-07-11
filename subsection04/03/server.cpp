#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT 8800
#define QUEUE 20
#define BUF_SIZE 10



int main()
{
    int sockSer = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(PORT);
    serAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockSer, (struct sockaddr*)&serAddr,
            sizeof(serAddr)) == -1) {
        printf("bind, err = %s\n", strerror(errno));
        return 0;
    }

    printf("listen %d port\n", PORT);

    if (listen(sockSer, QUEUE) == -1) {
        printf("listen, err = %s\n", strerror(errno));
        return 0;
    }

    char buf[BUF_SIZE];

    printf("wait for client\n");

    int conn;
    for (;;) {
        struct sockaddr_in clientAddr;
        socklen_t length = sizeof(clientAddr);
        conn = accept(sockSer, (struct sockaddr*)&clientAddr, &length);
        if (conn < 0) {
            printf("connect wrong!!\n");
        }
        printf("new client accepted.\n");
        pid_t childid;
        if((childid = fork()) == 0){
            printf("child process:%d created.\n", getpid());
            close(sockSer);
            pid_t pid = getpid();
            while (1) {
                memset(buf, 0, sizeof(buf));
                int len = recv(conn, buf, sizeof(buf), 0);
                if (len > 0) {
                    printf("message from client%d : %s", pid, buf);
                    memset(buf, 0, sizeof(buf));
                    strcpy(buf, "pong\n");
                    send(conn, buf, strlen(buf)+1, 0);
                    printf("send message to client:%s\n", buf);
                    break;
                }
                else {
                    printf("receive wrong!!\n");
                    break;
                }
            }
            close(conn);
            return 0;
        }
    }
    close(sockSer);
    return 0;
}
