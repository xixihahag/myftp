#include "error.h"
#include "network.h"
#include "ftp.h"
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

char re[MAXLINE];
char current_path[MAXLINE] = "/home/a/Desktop/ftp";
//解决cd浏览任意文件的问题
int indeep = 0;
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

int ftp_put_cd(int sockfd, char *para){
  char temp[MAXLINE];
  if( (strcmp(para, ".")) == 0)
    return 1;
  else if( (strcmp(para, "..")) == 0){
    if(indeep != 0){
      indeep--;
      memset(temp, 0, sizeof(temp));
      int i = strlen(current_path)-1;
      for(; i>=0; i--){
        if(current_path[i] == '/')
          break;
      }
      for(int j=0; j<i; j++)
        temp[j] = current_path[j];

      strcpy(current_path, temp);
    }
  }
  else{
    indeep++;
    strcat(current_path, "/");
    strcat(current_path, para);
  }
  printf("%s\n", current_path);

  return 1;
}
int ftp_put_put(int sockfd, char *para){
  //接收客户端传来的文件
  init();
  char filename[MAXLINE];
  char filepath[MAXLINE];
  int filefd;
  int filesize;

  for(int i=strlen(para)-1; i>=0; i--){
    if(para[i] == '/'){
      for(int j=0; i<strlen(para); j++)
        filename[j] = para[++i];
      break;
    }
  }

  strcat(filepath, current_path);
  strcat(filepath, "/");
  strcat(filepath, filename);

  // printf("para = %s\n", para);

  //调用O_TRUNC 新上传文件会覆盖旧文件
  if( (filefd = open(filepath, O_WRONLY|O_CREAT|O_TRUNC, 0777)) < 0){
    return err("ftp_put_put open file error\n");
  }
  else{
    printf("create file success\n");

    while(read(sockfd, buf, SENDFILESIZE) > 0){
      memcpy(&filesize, buf, 4);

      printf("filesize = %d\n", filesize);

      if( (write(filefd, buf+4, filesize)) < 0){
        close(filefd);
        return err("ftp_put_put write to file error\n");
      }

      //文件接收完毕
      if(filesize < (SENDFILESIZE-4))
        break;
    }
  }
  close(filefd);
  return 1;
}
int ftp_put_get(int sockfd, char *para){
  //向客户端传送文件
  init();
  char send[SENDFILESIZE];
  int filefd;
  int sendsize;
  char filepath[MAXLINE];
  strcat(filepath, current_path);
  strcat(filepath, "/");
  strcat(filepath, para);

  printf("serverfilepath = %s\n", filepath);


  if( (filefd = open(filepath, O_RDONLY)) == -1)
    return err("ftp_put_get open error\n");

  printf("open file success\n");

  //准备传输数据
  while( (sendsize = read(filefd, (send+4), (SENDFILESIZE-4))) > 0){

    printf("sendsize = %d\n", sendsize);

    memcpy(send, &sendsize, 4);
    if( (write(sockfd, send, SENDFILESIZE)) < 0){
      close(filefd);
      return err("put file error\n");
    }
    memset(send, 0, sizeof(send));
  }
  return 0;
}
int ftp_put_quit(int sockfd){
  printf("one people leave\n");
  return 1;
}
