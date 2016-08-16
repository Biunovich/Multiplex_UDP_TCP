#include "mylib.h"
int sock;
void *MulPlex(void *arg)
{
	char buf[MSGLEN];
	struct sockaddr_in addr_other;
	int struct_len = sizeof(addr_other);
	int  *sock_this = (int*)arg ,epfd, numfd,i;
	struct epoll_event ev ,events[CLIENT];
	epfd = epoll_create(1);
	ev.events = EPOLLIN;
	ev.data.fd = *sock_this;
	if (epoll_ctl(epfd,EPOLL_CTL_ADD, *sock_this,&ev) == -1)
		perror("epoll_ctl");
	while(1)
	{
		if ((numfd = epoll_wait(epfd,events,1,-1)) == -1)
			perror("epoll_wait");
		for (i=0;i<numfd;i++)
		{
			if (events[i].data.fd == *sock_this)
			{
				if (recvfrom(*sock_this,buf,MSGLEN,0,(struct sockaddr *)&addr_other,&struct_len) == -1)
				{
					perror("recvfrom");
				}
				if (sendto(*sock_this,buf,MSGLEN,0,(struct sockaddr *)&addr_other,struct_len) == -1)
				{
					perror("sendto");
				}
			}
		}
	}
}
void client_process()
{
	pid_t pid;
	pthread_t thread;
	char buf[MSGLEN];
	struct sockaddr_in addr_me ,addr_other;
	int struct_len = sizeof(addr_other),i,sock_new;
	if ((sock_new = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		perror("socket");
	}
	addr_me.sin_family = AF_INET;
	addr_me.sin_port = 0;
	inet_aton(IPADDR,&addr_me.sin_addr);
	if (bind(sock_new, (struct sockaddr *)&addr_me, sizeof(addr_me)) == -1)
	{
		perror("bind");
	}
	pthread_create(&thread,NULL,MulPlex,&sock_new);
	printf("%d\n",htons(addr_other.sin_port));
	for (i=0;i<CLIENT;i++)
	{
		printf("Waiting data\n");
		if (recvfrom(sock,buf,MSGLEN,0,(struct sockaddr *)&addr_other,&struct_len) == -1)
		{
			perror("recvfrom");
		}
		if (sendto(sock_new,"Connection",MSGLEN,0,(struct sockaddr *)&addr_other,struct_len) == -1)
		{
			perror("sendto");
		}
		printf("%s\n", buf);
	}
	pid = fork();
	if (pid == 0)
		client_process();
}
void main()
{
	pid_t pid;
	struct sockaddr_in addr_me;
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		perror("socket");
	}	
	addr_me.sin_family = AF_INET;
	addr_me.sin_port = htons(PORT);
	inet_aton(IPADDR,&addr_me.sin_addr);
	if (bind(sock, (struct sockaddr *)&addr_me, sizeof(addr_me)) == -1)
	{
		perror("bind");
	}
	pid = fork();
	if (pid == 0)
	{
		client_process();
	}
	wait(0);
	close(sock);
}