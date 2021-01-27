// =======================================================================
//  FileName:     unp.c
//  Author:       dingfang
//  CreateDate:   2021-01-27 19:05:03
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-27 22:10:04
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


