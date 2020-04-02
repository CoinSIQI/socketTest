#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
void sigfun(int signum)
{
    int status;
    
}
int main()
{
    pid_t pid;
    int i;
    sigaction(SIGCHLD,);
    //pid = fork();
    //sighandler_t ret = signal(SIGCHLD,sigfun);
    if(ret == SIG_ERR)
    {
        perror("signal");
    }
    for(i = 0;i<5;i++)
    {
        pid = fork();
        if(pid==0)
            break;
    }

    if(i<5)
    {
        printf("child process = %d\n",getpid());
        sleep(i);
        exit(0);
    }
    else
    {
        sleep(i);
        printf("parent process = %d\n",getppid());
        return 0;
    }
}
