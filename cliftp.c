#include "error.h"
#include "network.h"
#include "ftp.h"
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int n;
char buf[MAXLINE];
char recv_path[MAXLINE] = "./recvftp";

void init(){
  memset(buf, 0, sizeof(buf));
}

// void wait_4_server_reply(int sockfd){
//   init();
//   read(sockfd, buf, MAXLINE);
//   printf("%s\n", buf);
// }

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


int ftp_get_cd(int sockfd, char *cmd){
  //strlen 写成 sizeof 导致传输过去少一个字节，查了好半天
  if( (write(sockfd, cmd, strlen(cmd))) < 0)
    return err("ftp_get_cd error\n");
  return 1;
}

//上传文件
int ftp_get_put(int sockfd, char *cmd){
  init();
  char send[SENDFILESIZE];
  int filefd;
  int sendsize;
  if( (filefd = open(get_para(cmd, 4), O_RDONLY)) == -1)
    return err("ftp_get_put open error\n");

  // printf("open file success\n");

  //告诉服务器准备接受数据
  if( (write(sockfd, cmd, strlen(cmd))) < 0)
    return err("ftp_get_put write error\n");

  // printf("ready to send file\n");

  //准备传输数据
  while( (sendsize = read(filefd, (send+4), (SENDFILESIZE-4))) > 0){

    // printf("sendsize = %d\n", sendsize);

    memcpy(send, &sendsize, 4);
    if( (write(sockfd, send, SENDFILESIZE)) < 0){
      close(filefd);
      return err("put file error\n");
    }
    memset(send, 0, sizeof(send));
  }
  printf("upload file success\n");
  return 0;
}


int ftp_get_get(int sockfd, char *cmd){
  init();
  char *filename = get_para(cmd, 4);
  char filepath[MAXLINE];
  int filesize;
  int filefd;

  strcat(filepath, recv_path);
  strcat(filepath, "/");
  strcat(filepath, filename);

  if( (filefd = open(filepath, O_WRONLY|O_CREAT|O_TRUNC, 0777)) < 0){
    return err("ftp_get_get open file error\n");
  }

  if(write(sockfd, cmd, strlen(cmd)) < 0)
    return err("ftp_get_get write error\n");
  else{
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
  printf("file received success\n");
  return 0;
}


int ftp_get_quit(int sockfd){
  write(sockfd, "quit", sizeof("quit"));
  return 0;
}
