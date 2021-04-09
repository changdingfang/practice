// =======================================================================
//  FileName:     unp.c
//  Author:       dingfang
//  CreateDate:   2021-01-27 19:05:03
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-04-09 08:00:59
// =======================================================================

#include "unp.h"

#include <stdarg.h>
#include <syslog.h>


int daemon_proc;

static void err_doit(int, int, const char *, va_list);


void err_ret(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    err_doit(1, LOG_INFO, fmt, ap);
    va_end(ap);
    return ;
}


void err_sys(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    err_doit(1, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}


void err_dump(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    err_doit(1, LOG_ERR, fmt, ap);
    va_end(ap);
    abort();
    exit(1);
}


void err_msg(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    err_doit(0, LOG_INFO, fmt, ap);
    va_end(ap);
    return ;
}


void err_quit(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    err_doit(0, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}


static void err_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
    int errno_save, n;
    char buf[MAXLINE + 1];

    errno_save = errno;
    vsnprintf(buf, MAXLINE, fmt, ap);

    n = strlen(buf);
    if (errnoflag)
    {
        snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
    }
    strcat(buf, "\n");

    if (daemon_proc)
    {
        syslog(level, buf, strlen(buf));
    }
    else
    {
        fflush(stdout);
        fputs(buf, stderr);
        fflush(stderr);
    }
}


char *sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
    char portstr[8];
    static char str[128];

    switch (sa->sa_family)
    {
        case AF_INET: 
            {
                struct sockaddr_in *sin = (struct sockaddr_in *) sa;
                if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
                {
                    return NULL;
                }
                if (ntohs(sin->sin_port) != 0)
                {
                    snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
                    strcat(str, portstr);
                }
                return str;
            }
        case AF_INET6:
            {
                struct sockaddr_in6  *sin = (struct sockaddr_in6*)sa;
                if (inet_ntop(AF_INET, &sin->sin6_addr, str, sizeof(str)) == NULL)
                {
                    return NULL;
                }
                if (ntohs(sin->sin6_port) != 0)
                {
                    snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin6_port));
                    strcat(str, portstr);
                }
                return str;
            }
        default: return NULL;
    }
}


int Socket(int family, int type, int protocol)
{
    int n;

    if ((n = socket (family, type, protocol)) < 0)
    {
        err_sys("socket error");
    }

    return n;
}


int Bind(int fd, struct sockaddr *addr, socklen_t addrlen)
{
    return bind(fd, addr, addrlen);
}


int Listen(int fd, int backlog)
{
    return listen(fd, backlog);
}


int Accept(int fd, struct sockaddr *addr, socklen_t* addrlen)
{
    return accept(fd, addr, addrlen);
}


int Write(int fd, const char *buf, int buflen)
{
    return write(fd, buf, buflen);
}


int Close(int fd)
{
    return close(fd);
}


int Getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlen)
{
    if (getsockopt(fd, level, optname, optval, optlen) < 0)
    {
        err_quit("getsockopt error!");
    }

    return 0;
}


void Pthread_mutex_lock(pthread_mutex_t *mptr)
{
    int n;

    if ((n = pthread_mutex_lock(mptr)) == 0)
    {
        return ;
    }

    errno = n;
    err_sys("pthread_mutex_lock error");
}


ssize_t readn(int fd, char *ptr, size_t size)
{
    ssize_t aread   = 0;
    ssize_t nread   = 0;

    while (aread < size)
    {
        if ((nread = read(fd, ptr + aread, size - aread)) < 0)
        {
            if (nread == EINTR)
            {
                continue;
            }
            return -1;
        }
        else if (nread == 0)
        {
            break;
        }
        aread += nread;
    }

    return aread;
}


ssize_t writen(int fd, const char *ptr, size_t size)
{
    ssize_t awrite = 0;
    ssize_t nwrite = 0;

    while (awrite < size)
    {
        if ((nwrite = write(fd, ptr + awrite, size - awrite)) <= 0)
        {
            if (nwrite < 0 && nwrite == EINTR)
            {
                continue;
            }

            return -1;
        }

        awrite += nwrite;
    }

    return awrite;
}


ssize_t readline(int fd, char *ptr, size_t size)
{
    ssize_t aread = 1;
    ssize_t nread = 0;


    while (aread < size)
    {
        if ((nread = read(fd, ptr + aread, 1)) == 1)
        {
            aread += nread;
            if (*(ptr + aread) == '\n')
            {
                break;
            }
        }
        else if (nread == 0)
        {
            break;
        }
        else
        {
            if (nread == EINTR)
            {
                continue;
            }
            return -1;
        }
    }

    *(ptr + aread) = '\0';

    return aread;
}


void str_cli(FILE *fp, int sockfd)
{
    int maxfdpl;
    fd_set rset;
    char buf[MAXLINE];
    ssize_t len = 0;

    FD_ZERO(&rset);

    while (1)
    {
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);

        maxfdpl = max(fileno(fp), sockfd) + 1;

        int n = select(maxfdpl, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset))
        {
            if ((len = readn(sockfd, buf, MAXLINE)) == 0)
            {
                err_quit("str cli: server terminated prematurely");
            }
            writen(sockfd, buf, len);
        }

        if (FD_ISSET(fileno(fp), &rset))
        {
            if (readn(fileno(fp), buf, MAXLINE) == 0)
            {
                return ;
            }

            writen(sockfd, buf, strlen(buf));
        }
    }
}

struct addrinfo * hostServ(const char *host, const char *serv, int family, int socktype)
{
    int n;
    struct addrinfo hints, *res;

    memset(&hints, 0x00, sizeof(struct addrinfo));
    hints.ai_flags    = AI_CANONNAME;
    hints.ai_family   = family;
    hints.ai_socktype = socktype;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
    {
        return NULL;
    }

    return res;
}

int tcpConnect(const char *host, const char *serv)
{
    int sockfd, n;
    struct addrinfo hints, *res = NULL, *ressave = NULL;

    memset(&hints, 0x00, sizeof(struct addrinfo));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
    {
        err_quit("tcp connect error for %s, %s: %s", host, serv, gai_strerror(n));
    }

    ressave = res;

    while (res)
    {
        sockfd = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (socket < 0)
        {
            continue;
        }

        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
        {
            break;
        }
        Close(sockfd);
        res = res->ai_next;
    }

    if (res == NULL)
    {
        err_sys("tcp connect error for %s, %s", host, serv);
    }

    freeaddrinfo(ressave);

    return sockfd;
}

int tcpListen(const char *host, const char *serv, socklen_t *addrlenp)
{
    int listenfd, n;
    const int on = 1;
    struct addrinfo hints, *res, *ressave;

    memset(&hints, 0x00, sizeof(struct addrinfo));
    hints.ai_flags    = AI_PASSIVE;
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
    {
        err_quit("tcp connect error for %s, %s: %s", host, serv, gai_strerror(n));
    }

    ressave = res;

    while (res)
    {
        listenfd = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (listenfd < 0)
        {
            continue;
        }

        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
        {
            break;
        }
        Close(listenfd);
        res = res->ai_next;
    }

    if (res == NULL)
    {
        err_sys("tcp connect error for %s, %s", host, serv);
    }

    Listen(listenfd, LISTENQ);

    if (addrlenp)
    {
        *addrlenp = res->ai_addrlen;
    }

    freeaddrinfo(ressave);

    return listenfd;
}

char *gfTime(void)
{
    struct timeval tv;
    static char str[30];
    char *ptr;

    if (gettimeofday(&tv, NULL) < 0)
    {
        err_sys("gettiemofday error");
    }

    ptr = ctime(&tv.tv_sec);
    /* xxx xxx xx 00:00:00 2021\n\0*/
    strcpy(str, &ptr[11]);

    snprintf(str + 8, sizeof(str) - 8, ".%06ld", tv.tv_usec);

    return str;
}
