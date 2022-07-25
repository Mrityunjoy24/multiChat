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
#include<pthread.h>


static volatile int bal=0;

int port;
int i=0,p,m;
int newfd[50];

void *handler(void *ptr)
{
	char ch[2];
	int sock=*(int *)ptr,jj;
	char buf[1000];
	char b[10];
	memset(&b,'\0',sizeof(b));
	memset(&buf,'\0',sizeof(buf));
	//bal=0;
	for(int i=0;i<15000+sock;i++)
		bal++;
	while((recv(sock,buf,1000,0)))
	{
 	if(strcmp(buf,"end")==0)
 	{
 		printf("client disconnected\n");
 		return 0;
 	}
 	
 	printf("client sent::%s\n",buf);
 	int count=1;
 	while((recv(sock,b,10,0)))
 	{
 		printf("forwarding to client %s\n",b);
 		count--;
 		if(count==0)
 			break;
 	}

 
 	int i=atoi(b);

 	send(newfd[i],buf,strlen(buf),0);
 	memset(&buf,'\0',sizeof(buf));
 	memset(&b,'\0',sizeof(b));
 	}
 	free(ptr);
 	return 0;
}

void *serv_send(int *p)
{
	int ii;
	char buffer[1000];
	char c;
	
	while(1)
	{
		memset(&buffer,'\0',sizeof(buffer));		
		scanf("%s",buffer);
		if(strcmp(buffer,"q")==0)
		{
			printf("disconnecting---\n");
			exit(1);
		}
		printf("\nenter index to send::");
		scanf("%d",&ii);
		send(*(p+ii),buffer,strlen(buffer),0);
	}
}

int main(int argc, char *argv[])
{
	char buffer[50];
	port = atoi(argv[1]);
	int sockfd,*ne,*ge;
	struct sockaddr_in servaddr,newaddr;
	pid_t p;
	socklen_t sin_size;
	pthread_t tid1,tid2;

	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		printf("unable to create socket\n");
		exit(1);
	}
	printf("server socket created\n");
	memset(&servaddr,'\0',sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(port);
	servaddr.sin_addr.s_addr=INADDR_ANY;
	//inet_pton(AF_INET,argv[2],&(servaddr.sin_addr));

	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1)
 	{
 		close(sockfd);
 		printf("failed to bind");
 		exit(1);
 	}
 	printf("bind sucessful\n");

 	if(listen(sockfd,10)==0)
 	{
 		printf("listening---\n");
 		printf("main() starts depositing, balance = %d\n", bal);
 	}
 	else
 		printf("error in listening\n");
 	sin_size=sizeof(newaddr);

 	 //printf("main() starts depositing, balance = %d\n", balance);
 	pthread_create(&tid2,NULL,serv_send,newfd);

 	int m=1;
 	while((newfd[i]=accept(sockfd,(struct sockaddr *)&newaddr,&sin_size)))
 	{
 		printf("connection established--\n");
 		ne=malloc(1);
 		*ne=newfd[i];
 		
 		
 		
 		sprintf(buffer,"you are client %d",i);
 		send(newfd[i],buffer,strlen(buffer),0);
 		i+=1;
 		pthread_create(&tid1,NULL,handler,(void *)ne);
 		if(m++==2)
 			break;
 	}
 	//bal=900;
 	printf("main() starts depositing, balance = %d\n", bal);
 	if(newfd[i]<0)
 		exit(1);
 	return 0;
 }
