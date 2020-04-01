//server.cpp epoll
//
//
#include "inet.h"
//#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
int process_rw(void* arg)
{
    int newfd = *(int *)arg;
    char recvbuf[128];
    int len = 0;
    
    while((len = read(newfd,recvbuf,128))>0)
    {
        printf("server recv fd=%d data %s\n",newfd,recvbuf);
        if(write(newfd,recvbuf,strlen(recvbuf))<0)
        {
             perror("write");
        }
        
    }
    if(len == 0)
    {
        printf("client fd =%d disconnect\n",newfd);
    }
    else
    {
        perror("read");
        return -1;
    }
    //close(newfd);
    return 0;
}
void sigfun(int signum)
{
    pid_t pid;
    int stat;
    while((pid == waitpid(-1,&stat,WNOHANG))>0)
    {
        
    }
}
int main()
{
	//create socket
	int ret;
	//int maxfd;
//	char str[] ="recv data:";
	//int j =0;
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("socket");
		exit(1);
	}
	//addr reuser
	int bres = 1;
	ret = setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char*)&bres,sizeof(int));
	if(ret == -1)
	{
		perror("setsockopt");
	}
	//bind
	sockaddr_in saddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(PORT_USER);
	saddr.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sock,(struct sockaddr*)&saddr,sizeof(struct sockaddr));
	if(ret == -1)
	{
		perror("bind");
		exit(1);
	}
	//listen socket
	ret = listen(sock,5);
	if(ret==-1)
	{
		perror("bind");
		exit(1);
	}
	printf("listen success.\n");
    sockaddr_in caddr;
    int caddrlen = sizeof(caddr);
    int newfd;
    signal(17,sigfun);
    pid_t pid;
	while(1)
	{
        newfd = accept(sock,(struct sockaddr*)&caddr,(socklen_t*)&caddrlen);
        if(newfd<0)
        {
            perror("accept");
            continue;
        }
        printf("accept new client IP=%s\n",inet_ntoa(caddr.sin_addr));
        pid = fork();
        if(pid == 0)
        {
            close(sock); //关闭监听套接字
            ret = process_rw((void *)&newfd);
            if(ret != 0)
            {
                printf("fd = %d read error\n",newfd);
            }
            close(newfd);
            exit(0);
        }
        close(newfd);
    }
	close(sock);
	return 0;
}
