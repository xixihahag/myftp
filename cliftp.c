#include "error.h"
#include "network.h"
#include "ftp.h"
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>

int n;
char buf[MAXLINE];

void init(){
  memset(buf, 0, sizeof(buf));
}
//--------------------------------------------------
//客户端请求函数
int ftp_get_help(int sockfd){
  init();
  write(sockfd, "help", sizeof("help"));
  if( (n = read(sockfd, buf, MAXLINE)) > 0)
    write(fileno(stdout), buf, sizeof(buf));
  if(n < 0)
    return err("ftp_get_help error\n");
}


int ftp_get_ls(int sockfd){
  init();
  write(sockfd, "ls", sizeof("ls"));
  if( (n = read(sockfd, buf, MAXLINE)) > 0)
    write(fileno(stdout), buf, sizeof(buf));
  if(n < 0)
    return err("ftp_get_ls error\n");
}
int ftp_get_cd(int sockfd){
  return 0;
}
int ftp_get_put(int sockfd){
  return 0;
}
int ftp_get_get(int sockfd){
  return 0;
}
int ftp_get_quit(int sockfd){
  return 0;
}
