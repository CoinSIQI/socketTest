#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <error.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
int main()
{
	///hello world  
	int fd = open("./aa",O_RDWR | O_CREAT,0644);
    if(fd<0)
    {
        perror("open");
        exit(1);
    }
    int ret = ftruncate(fd,4096);
    if(ret<0)
    {
        perror("ftruncate");
        exit(1);
    }
    char *p;
    p = (char *)mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(p==MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    char buf[1024];
    //int flag = fcntl(STDIN_FILENO,F_GETFL);
    //flag |= O_NONBLOCK;
    //fcntl(STDIN_FILENO,F_SETFL,flag);
    while(1)
    {
        //read(STDIN_FILENO,buf,1024);
        //fgets();
        //strcpy(p,buf);
        printf("%s\n",p);
        sleep(1);
    }
    munmap(p,4096);
    printf("hello world!\n");
	return 0;
}
