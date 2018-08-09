#include "error.h"
#include "ftp.h"
#include "network.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  int sockfd;
  char buf[MAXLINE];
  int cmd, end;

  if( (sockfd = network_init(IS_CLIENT)) == -1){
    perr("client network_init error\n");
    exit(EXIT_FAILURE);
  }

  for(;;){
    if(end == 1)
      break;
    printf("please input cmd\n");
    memset(buf, 0, sizeof(buf));
    read(fileno(stdin), buf, MAXLINE);
    buf[strlen(buf)-1] = '\0';
    cmd = ana_cmd(buf);
    switch (cmd) {
      case CMD_HELP:
        if( (ftp_get_help(sockfd)) == -1)
          perr("ftp_help error\n");
        break;
      case CMD_LS:
        if( (ftp_get_ls(sockfd)) == -1)
          perr("ftp_ls error\n");
        break;
      case CMD_CD:
        if ( (ftp_get_cd(sockfd)) == -1)
          perr("ftp_cd error\n");
        break;
      case CMD_PUT:
        if( (ftp_get_put(sockfd)) == -1)
          perr("ftp_put error\n");
        break;
      case CMD_GET:
        if( (ftp_get_get(sockfd)) == -1)
          perr("ftp_get error\n");
        break;
      case CMD_QUIT:
        // if( (ftp_get_quit(sockfd)) == -1)
        //   perr("ftp_quit error\n");
        close(sockfd);
        end = 1;
        break;
      default: printf("cmd error please input again\n");
    }
  }
  // printf("welcome next time\n");

  return 0;
}
