#include "mylib.h"
void main()
{

	struct sockaddr_in addr_me_TCP ,addr_other_TCP;
	int sock_TCP,struct_len = sizeof(addr_other_TCP), sock_clt;
	char buf[MSGLEN];
	if ((sock_TCP = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1)
	{
		perror("Socket");
	}
	addr_me_TCP.sin_family = AF_INET;
	addr_me_TCP.sin_port = htons(PORT1);
	inet_aton(IPADDR,&addr_me_TCP.sin_addr);
	if ((bind(sock_TCP,(struct sockaddr*)&addr_me_TCP,struct_len)) == -1)
	{
		perror("bind");
	}
	listen(sock_TCP,CLIENT);


	struct sockaddr_in addr_me_UDP ,addr_other_UDP;
	int sock_UDP;
	if ((sock_UDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		perror("socket");
	}
	memset(&addr_me_UDP,0,sizeof(addr_me_UDP));
	addr_me_UDP.sin_family = AF_INET;
	addr_me_UDP.sin_port = htons(PORT);
	inet_aton(IPADDR,&addr_me_UDP.sin_addr);
	if (bind(sock_UDP, (struct sockaddr *)&addr_me_UDP, sizeof(addr_me_UDP)) == -1)
	{
		perror("bind");
	}

	struct pollfd fds[2];
	fds[0].fd = sock_TCP;
	fds[0].events = POLLIN;
	fds[1].fd = sock_UDP;
	fds[1].events = POLLIN;
	int retval; 
	while(1)
	{
		retval = poll((struct pollfd *)&fds,2,0);
		if (retval == -1)
			perror("poll");
		else
		{
			if (fds[0].revents & POLLIN)
			{
				fds[0].revents = 0;
				if ((sock_clt = accept(sock_TCP,(struct sockaddr*)&addr_other_TCP,&struct_len)) == -1)
				{
					perror("accept");
				}
				printf("Waiting from TCP\n");
				if (recv(sock_clt,buf,MSGLEN,0) == -1)
				{
					perror("recv");
				}
				puts(buf);
				if (send(sock_clt,buf,MSGLEN,0) == -1)
				{
					perror("send");
				}
			}
			if (fds[1].revents & POLLIN)
			{
				fds[1].revents = 0;
				printf("Waiting data from UDP\n");
				if (recvfrom(sock_UDP,buf,MSGLEN,0,(struct sockaddr *)&addr_other_UDP,&struct_len) == -1)
				{
					perror("recvfrom");
				}
				printf("%s\n", buf);
				if (sendto(sock_UDP,buf,MSGLEN,0,(struct sockaddr *)&addr_other_UDP,struct_len) == -1)
				{
					perror("sendto");
				}
			}
		}
	}
	close(sock_UDP);
	close(sock_TCP);
	close(sock_clt);
}