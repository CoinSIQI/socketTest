//server.cpp epoll
//
//
#include <inet.h>

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
//	int fdnum[1024];
	//fdnum[sock] = sock;
	//accept clients
	char recvbuf[5] = {0};
	char sendbuf[140];
	//maxfd = sock;
	int epfd = epoll_create(1000);//create epoll tree root node
	if(epfd<0)
	{
		perror("epoll_create");
		exit(1);
	}
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = sock;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,sock,&ev);
	if(ret == -1)
	{
		perror("epoll_ctl");
		exit(1);
	}
	int i,temp_fd,fdnum,newfd;
	struct epoll_event all[1000];
	while(1)
	{
		printf("---------epoll_wait-----------\n");
		fdnum = epoll_wait(epfd,all,1000,-1);
		if(fdnum<0)
		{
			perror("epoll_wait");
		}
		for(i=0;i<fdnum;++i)
		{
			 temp_fd = all[i].data.fd;
			 if(temp_fd==sock)
			 {
				sockaddr_in caddr;
				int caddrlen = sizeof(caddr);
				newfd = accept(sock,(struct sockaddr*)&caddr,(socklen_t*)&caddrlen);
				if(newfd<0)
				{
					perror("accept");
					continue;
				}
				printf("accept new client IP=%s\n",inet_ntoa(caddr.sin_addr));
				int flag = fcntl(newfd,F_GETFL);
				flag |= O_NONBLOCK;
				fcntl(newfd,F_SETFL,flag);
				struct epoll_event newev;
			    newev.events = EPOLLIN | EPOLLET;
				newev.data.fd = newfd;
				ret = epoll_ctl(epfd,EPOLL_CTL_ADD,newfd,&newev);
			 }
			 else
			 {
				memset(recvbuf,0,sizeof(recvbuf));
				ret = 1;
				while(ret>0)
				{
					ret = read(temp_fd,recvbuf,sizeof(recvbuf));
					write(STDOUT_FILENO,recvbuf,strlen(recvbuf));
					//recvbuf = str+recvbuf;
				//	send(temp_fd,recvbuf,strlen(recvbuf),0);
				}
				printf("\n");
				if(ret == 0)
				{
					printf("client disconnect...\n");
					epoll_ctl(epfd,EPOLL_CTL_DEL,temp_fd,NULL);
					close(temp_fd);
				}
				else
				{
					if(errno == EAGAIN)
					{
						printf("data read cpmplete.\n");
					}
					else
					{
						perror("read");
						continue;
					}
				}

			 }
		}
	}
	close(sock);
	return 0;
}
