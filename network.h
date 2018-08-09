#define LISTENQ 1024
#define SERV_PORT 666
#define SA struct sockaddr
#define IP_ADDRESS "127.0.0.1"
#define INADDR_ANY "0.0.0.0"
#define CMD_SIZE 1024
#define MAXLINE 10240

#define IS_SERVER 0
#define IS_CLIENT 1

#define CMD_HELP 00
#define CMD_LS 11
#define CMD_CD 22
#define CMD_PUT 33
#define CMD_GET 44
#define CMD_QUIT 55

int network_init(int);
int ana_cmd(char *);
