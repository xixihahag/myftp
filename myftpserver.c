#include "network.h"
#include "ftp.h"
#include "error.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  int sockfd, connfd;
  char cmd[CMD_SIZE];
  int result, n;

  if( (sockfd = network_init(IS_SERVER)) == -1)
    exit(EXIT_FAILURE);

  for(;;){
    if( (connfd = accept(sockfd, NULL, NULL)) == -1){
      perr("accept error\n");
      break;
    }
    // printf("accept success\n");
    for(;;){
      memset(cmd, 0, sizeof(cmd));
      if( (n = read(connfd, cmd, CMD_SIZE)) < 0){
        perr("read error\n");
        break;
      }else if(n == 0){
        //客户端close关闭连接
        printf("one people leave\n");
        break;
      }else{
        result = ana_cmd(cmd);
        switch(result){
          case CMD_HELP:
            ftp_put_help(connfd);
            break;
          case CMD_LS:
            ftp_put_ls(connfd);
            break;
          case CMD_CD:
            ftp_put_cd(connfd, get_para(cmd, 3));
            break;
          case CMD_PUT:
            ftp_put_put(connfd, get_para(cmd, 4));
            break;
          case CMD_GET:
            ftp_put_get(connfd, get_para(cmd, 4));
            break;
        }
      }
    }
  }

  return 0;
}
