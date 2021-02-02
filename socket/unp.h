// =======================================================================
//  FileName:     unp.h
//  Author:       dingfang
//  CreateDate:   2021-01-27 08:42:50
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-27 22:11:06
// =======================================================================

#ifndef __UNP_H__
#define __UNP_H__

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/sysctl.h>
#include <poll.h>
#include <event.h>
#include <sys/ioctl.h>
#include <pthread.h>


#define LISTENQ     1024

#define MAXLINE     4096
#define BUFFSIZE    8192

#define SERV_PORT       9877
#define SERV_PORT_STR   "9877"
#define UNIXSTR_PATH    "/tmp/unix.str"
#define UNIXDG_PATH     "/tmp/unix.dg"

#define SA struct sockaddr

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))



void err_ret(const char *fmt, ...);
void err_sys(const char *fmt, ...);
void err_dump(const char *fmt, ...);
void err_msg(const char *fmt, ...);
void err_quit(const char *fmt, ...);


int Socket(int family, int type, int protocol);
int Bind(int fd, struct sockaddr *addr, socklen_t addrlen);
int Listen(int fd, int backlog);
int Accept(int fd, struct sockaddr *addr, socklen_t* addrlen);
int Write(int fd, const char *buf, int buflen);
int Close(int fd);

void Pthread_mutex_lock(pthread_mutex_t *mptr);


#endif /* __UNP_H__ */
