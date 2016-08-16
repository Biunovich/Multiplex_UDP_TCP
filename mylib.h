#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <error.h>
#include <sys/wait.h>
#define MSGLEN 512
#define PORT 1337
#define PORT1 1488
#define IPADDR "127.0.0.1"
#define CLIENT 10
#define BCASTIP "192.168.2.255"
#define MCASTIP "239.0.0.11"
