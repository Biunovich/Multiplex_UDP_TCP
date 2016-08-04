#include "mylib.h"
void main()
{
	struct sockaddr_in addr_other;
	char buf[MSGLEN];
	int sock , struct_len = sizeof(addr_other);
	if ((sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1)
	{
		perror("socket");
	}
	addr_other.sin_family = AF_INET;
	addr_other.sin_port = htons(PORT1);
	inet_aton(IPADDR,&addr_other.sin_addr);
	if (connect(sock,(struct sockaddr*)&addr_other,struct_len) == -1)
	{
		perror("connect");
		close(sock);
	}
	while(strcmp(buf,"quit") != 0)
	{
		printf("ENTER MESSAGE: ");
		scanf("%s",buf);
		if (send(sock,buf,MSGLEN,0) == -1)
		{
			perror("send");
		}
		if (recv(sock,buf,MSGLEN,0) == -1)
		{
			perror("recv");
		}
		printf("%s\n", buf);
	}
	close(sock);
}