#include "mylib.h"
int sock;
void client_process(struct sockaddr_in addr_other)
{
	close(sock);
	char buf[MSGLEN];
	struct sockaddr_in addr_me;
	int struct_len = sizeof(addr_me);
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		perror("socket");
	}
	addr_me.sin_family = AF_INET;
	addr_me.sin_port = 0;
	inet_aton(IPADDR,&addr_me.sin_addr);
	if (bind(sock, (struct sockaddr *)&addr_me, sizeof(addr_me)) == -1)
	{
		perror("bind");
	}
	if (sendto(sock,"New port : ",MSGLEN,0,(struct sockaddr *)&addr_other,struct_len) == -1)
	{
		perror("sendto");
	}
	printf("%d\n",htons(addr_other.sin_port));
	while(1)
	{
		printf("Waiting data\n");
		if (recvfrom(sock,buf,MSGLEN,0,(struct sockaddr *)&addr_other,&struct_len) == -1)
		{
			perror("recvfrom");
		}
		printf("%s\n", buf);
		if (sendto(sock,buf,MSGLEN,0,(struct sockaddr *)&addr_other,struct_len) == -1)
		{
			perror("sendto");
		}
	}
	close(sock);
}
void main()
{
	char buf[MSGLEN];
	pid_t pid;
	struct sockaddr_in addr_me ,addr_other;
	int  struct_len = sizeof(addr_other),num=0;
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
	while(1)
	{
		if (recvfrom(sock,buf,MSGLEN,0,(struct sockaddr *)&addr_other,&struct_len) == -1)
		{
			perror("recvfrom");
		}
		pid = fork();
		if (pid == 0)
		{
			num=0;
			while(num<100){
			if (recvfrom(sock,buf,MSGLEN,0,(struct sockaddr *)&addr_other,&struct_len) == -1)
			{
				perror("recvfrom");
			}
			client_process(addr_other);
			num++;
			}
		}
	}
	close(sock);
}