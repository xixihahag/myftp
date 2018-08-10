#define SENDFILESIZE 1024

int ftp_get_help(int);
int ftp_get_ls(int);
int ftp_get_cd(int, char*);
int ftp_get_put(int, char*);
int ftp_get_get(int, char*);
int ftp_get_quit(int);

int ftp_put_help(int);
int ftp_put_ls(int);
int ftp_put_cd(int, char*);
int ftp_put_put(int, char*);
int ftp_put_get(int, char*);
int ftp_put_quit(int);

void show_help();
void init();
void show_help();
