#include "error.h"
#include "network.h"
#include "ftp.h"
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>

char re[MAXLINE];
char current_path[MAXLINE] = "/home/a/Desktop/ftp";
DIR *dp;
struct dirent *entry;
int n;
//用于从服务器接收信息
char buf[MAXLINE];

void show_help(){
  // sprintf(re, "\tls---------------显示服当前目录下的文件\n");
  // sprintf(re, "\tcd path----------进入下一个目录\n");
  // sprintf(re, "\tget--------------下载文件\n");
  // sprintf(re, "\tput--------------上传文件\n");
  // sprintf(re, "\tquit--------------退出\n");

  //中文乱码，写个英文版
  sprintf(re, "\tls---------------Display the files in the current directory\n");
  strcat(re, "\tcd path----------Go to the next directory\n");
  strcat(re, "\tget--------------download file\n");
  strcat(re, "\tput--------------upload files\n");
  strcat(re, "\tquit-------------exit\n");
  // sprintf(re, "\tcd path----------Go to the next directory\n");
  // sprintf(re, "\tget--------------download file\n");
  // sprintf(re, "\tput--------------upload files\n");
  // sprintf(re, "\tquit-------------exit\n");
}

void init(){
  memset(re, 0, sizeof(re));
}


//-----------------------------------------------------------
//服务器端应答函数
int ftp_put_help(int sockfd){
  init();
  show_help();
  if( (write(sockfd, re, sizeof(re))) < 0)
    return err("ftp_help write error\n");
  return 1;
}

int ftp_put_ls(int sockfd){
  init();
  if( (dp = opendir(current_path)) != NULL){
    while( (entry = readdir(dp)) != NULL){
      if( (strcmp(entry->d_name, ".") == 0) || \
        (strcmp(entry->d_name, "..") == 0) )
        continue;
      strcat(re, entry->d_name);
      strcat(re, "\t");
    }
    strcat(re, "\n");
    if( (write(sockfd, re, sizeof(re))) < 0){
      return err("ftp_ls write error\n");
    }
  }
}

int ftp_put_cd(int sockfd){
  return 0;
}
int ftp_put_put(int sockfd){
  return 0;
}
int ftp_put_get(int sockfd){
  return 0;
}
int ftp_put_quit(int sockfd){
  return 0;
}
