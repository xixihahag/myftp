#include "network.h"
#include "error.h"
#include <string.h>
#include <sys/socket.h>
#include	<sys/types.h>
#include <arpa/inet.h>     //sockaddr_in
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */

int network_init(int type){
  int sockfd;
  struct sockaddr_in servaddr;

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);

  if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    return err("socket error\n");
  }

  if(type == IS_SERVER){
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if( (bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) == -1){
      return err("bind error\n");
    }
    if( (listen(sockfd, LISTENQ)) == -1){
      return err("listen error\n");
    }
  }else if(type == IS_CLIENT){
    servaddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    if( (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))) == -1){
      return err("connect error\n");
    }
  }

  return sockfd;
}

int ana_cmd(char *cmd){
  if( (strcmp(cmd, "help")) == 0){
    //帮助命令，显示所有可用命令
    return CMD_HELP;
  }
  if( (strcmp(cmd, "ls")) == 0){
    //显示服务器上的文件
    return CMD_LS;
  }
  if( (strncmp(cmd, "cd ", 3)) == 0){
    //进入下一个文件夹
    return CMD_CD;
  }
  if( (strncmp(cmd, "get ", 4)) == 0){
    //下载文件
    return CMD_GET;
  }
  if( (strncmp(cmd, "put ", 4)) == 0){
    //上传文件
    return CMD_PUT;
  }
  if( (strcmp(cmd, "quit")) == 0){
    return CMD_QUIT;
  }
}
