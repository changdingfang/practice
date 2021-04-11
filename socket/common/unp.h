// =======================================================================
//  FileName:     unp.h
//  Author:       dingfang
//  CreateDate:   2021-01-27 08:42:50
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-04-11 10:48:20
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
#include <netinet/tcp.h>
#include <net/if.h>


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


char *sock_ntop(const struct sockaddr *sa, socklen_t salen);


int Socket(int family, int type, int protocol);
int Bind(int fd, struct sockaddr *addr, socklen_t addrlen);
int Listen(int fd, int backlog);
int Accept(int fd, struct sockaddr *addr, socklen_t* addrlen);
int Write(int fd, const char *buf, int buflen);
int Close(int fd);

int Getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlen);


void Pthread_mutex_lock(pthread_mutex_t *mptr);

ssize_t readn(int , char *, size_t );
ssize_t writen(int , const char *, size_t );
ssize_t readline(int , char *, size_t );


void str_cli(FILE *fp, int sockfd);

struct addrinfo * hostServ(const char *host, const char *serv, int family, int socktype);
int tcpConnect(const char *host, const char *serv);
int tcpListen(const char *host, const char *serv, socklen_t *addrlenp);

char *gfTime(void);


#define IFI_NAME    16
#define IFI_HADDR   8

#if 0
#define IFNAMSIZ    16
struct ifconf 
{
    long int ifc_len;
    union {
        caddr_t ifcu_buf;
        struct ifreq *ifcu_req;
    }ifc_ifcu;
};

#define ifc_buf ifc_ifcu.ifcu_buf
#define ifc_req ifc_ifcu.ifc_req;

struct ifreq {
    char ifr_name[IFNAMSIZ];
    union {
        struct sockaddr ifru_addr;
        struct sockaddr ifru_dstaddr;
        struct sockaddr ifru_broadaddr;
        short ifru_flags;
        int ifru_metric;
        caddr_t ifru_data;
    } ifr_ifru;
};

#define ifr_addr        ifr_ifru.ifru_addr
#define ifr_dstaddr     ifr_ifru.ifru_dstaddr
#define ifr_broadaddr   ifr_ifru.ifru_broadaddr
#define ifr_flags       ifr_ifru.ifru_flags
#define ifr_metric      ifr_ifru.ifru_metric
#define ifr_data        ifr_ifru.ifru_data
#endif

struct ifi_info 
{
    char    ifi_name[IFI_NAME];
    short   ifi_index;
    short   ifi_mtu;
    u_char  ifi_haddr[IFI_HADDR];
    u_short ifi_hlen;
    short   ifi_flags;
    short   ifi_myflags;
    struct sockaddr     *ifi_addr;
    struct sockaddr     *ifi_brdaddr;
    struct sockaddr     *ifi_dstaddr;
    struct ifi_info     *ifi_next;
};

#define IFI_ALIAS   1

struct ifi_info *get_ifi_info(int, int);

void free_ifi_info(struct ifi_info *);

#endif /* __UNP_H__ */

