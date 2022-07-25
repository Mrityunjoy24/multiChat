#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>

int port;

void *sender(void *ptr)
{
	int sfd=*(int *)ptr;
	char buf[1000];
	while(1)
	{
		memset(&buf,'\0',sizeof(buf));
		
 		gets(buf);
 		send(sfd,buf,strlen(buf),0);
		if(strcmp(buf,"bye")==0)
 	 	{
 	 		close(sfd);
 	 		free(ptr);
 	 		printf("disconnected\n");
 	 		exit(1);
 	 	}
 		memset(&buf,'\0',sizeof(buf));
 		gets(buf);
 		send(sfd,buf,strlen(buf),0);
 		memset(&buf,'\0',sizeof(buf));
 	}
}

void *receiver(void *p)
{
	int sfd=*(int *)p;
	char buf[1000];
	memset(&buf,'\0',sizeof(buf));
	while((recv(sfd,buf,1000,0)))
 	{
 		printf("msg received from %s\n",buf);
 		memset(&buf,'\0',sizeof(buf));
 	}
 	if((recv(sfd,buf,1000,0))==0)
 	{
 		printf("server disconnected\n");
 		exit(1);
 	}

}


int main(int argc, char *argv[])
{
	port = atoi(argv[1]);
	int sockfd,*ne;
	pthread_t tid1,tid2;
	struct sockaddr_in servaddr;
	char buf[1000];
	memset(&buf,'\0',sizeof(buf));
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		printf("unable to create socket\n");
		exit(1);
	}
	printf("client socket created\n");
	memset(&servaddr,'\0',sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(port);
	inet_pton(AF_INET,argv[2],&(servaddr.sin_addr));
	if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1)
 	{
 		close(sockfd);
 		printf("error in connection\n");
 		exit(1);
 	}
 	printf("connected to server---\n");
 	ne=malloc(1);
 	*ne=sockfd;
 	pthread_create(&tid1,NULL,sender,(void *)ne);
 	pthread_create(&tid2,NULL,receiver,(void *)ne);
 	pthread_join(tid1,0);
 	pthread_join(tid2,0);
 	return 0;
}
