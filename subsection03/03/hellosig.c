#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


static void SigPrint(int signo)
{
    switch(signo){
        case 3:
        case 15:
            printf("bye\n");exit(0);break;
        case 14:
            printf("hello\n");alarm(1);break;
    }
}



int main()
{
    signal(SIGALRM, SigPrint);
    SigPrint(SIGALRM);

    if (signal(SIGQUIT, SigPrint) == SIG_ERR)
        printf("can't use SIGQUIT\n");

    if (signal(SIGTERM, SigPrint) == SIG_ERR)
        printf("can't use SIGTERM\n");

    while(1)
        sleep(10);

    return 0;
}
