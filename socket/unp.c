// =======================================================================
//  FileName:     unp.c
//  Author:       dingfang
//  CreateDate:   2021-01-27 19:05:03
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-02-07 21:04:17
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
