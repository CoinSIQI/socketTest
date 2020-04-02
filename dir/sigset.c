#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
void pendprintf(sigset_t* set)
{
    printf("=====================\n");
    for(int i = 0;i<32;i++)
    {
        if(sigismember(set,i)==1)
        {
            putchar('1');
        }
        else
        {
            putchar('0');
        }
    }
    printf("\n");
}
int main()
{
    
#if 0
    int sigemptyset(sigset_t *set);
    int sigfillset(sigset_t *set);
    int sigaddset(sigset_t *set, int signum);
    int sigdelset(sigset_t *set, int signum);
    int sigismember(const sigset_t *set, int signum);
#endif
    sigset_t sigset,oldset,pend;
    int ret;
    ret = sigemptyset(&sigset);
    if(ret==-1)
    {
        perror("sigempytset");
        exit(1);
    }
    ret = sigaddset(&sigset,SIGALRM);
    if(ret==-1)
    {
        perror("sigaddset");
        exit(1);
    }
    ret = sigaddset(&sigset,SIGQUIT);
    if(ret==-1)
    {
        perror("sigaddset");
        exit(1);
    }
    ret = sigismember(&sigset,SIGALRM);
    if(ret == 1)
    {
        printf("SIGALRM is exist\n");
    }else
    {
        perror("sigismember");
    }
    ret = sigprocmask(SIG_BLOCK, &sigset, &oldset);
    if(ret==-1)
    {
        perror("sigprocmask");
    }
    alarm(2);
    while(1)
    {
        sigpending(&pend);
        pendprintf(&pend);
       // printf("-------------\n");
        sleep(1);
    }
    return 0;
}
