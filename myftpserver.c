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

  if( (sockfd = network_init(IS_SERVER)) == -1){
    perr("network_init error\n");
    exit(EXIT_FAILURE);
  }
  // printf("network_init success\n");
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
        printf("online people --\n");
        break;
      }else{
        result = ana_cmd(cmd);
        switch(result){
          case CMD_HELP:
            if( (ftp_put_help(connfd)) == -1)
              perr("ftp_help error\n");
            break;
          case CMD_LS:
            if( (ftp_put_ls(connfd)) == -1)
              perr("ftp_ls error\n");
            break;
          case CMD_CD:
            if ( (ftp_put_cd(connfd, get_para(cmd, 3))) == -1)
              perr("ftp_cd error\n");
            break;
          case CMD_PUT:
            if( (ftp_put_put(connfd, get_para(cmd, 4))) == -1)
              perr("ftp_put error\n");
            break;
          case CMD_GET:
            if( (ftp_put_get(connfd, get_para(cmd, 4))) == -1)
              perr("ftp_get error\n");
            break;
          // case CMD_QUIT:
          //   if( (ftp_put_quit(connfd)) == -1)
          //     perr("ftp_quit error\n");
          //   break;
          // default: printf("cmd error please input again\n");
        }
      }
    }
  }


  return 0;
}
