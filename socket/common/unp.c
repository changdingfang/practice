// =======================================================================
//  FileName:     unp.c
//  Author:       dingfang
//  CreateDate:   2021-01-27 19:05:03
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-04-29 20:23:03
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


char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen)
{
    static char str[128];

    switch (sa->sa_family)
    {
        case AF_INET: 
            {
                struct sockaddr_in *sin = (struct sockaddr_in *)sa;

                if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
                {
                    return NULL;
                }
                return str;
            }
#ifdef IPV6
        case AF_INET6:
            {
                struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)sa;

                if (inet_ntop(AF_INET6, &sin6->sin_addr, str, sizeof(str)) == NULL)
                {
                    return NULL;
                }

                return str;
            }
#endif

#ifdef AF_UNIX
        case AF_UNIX:
            {
                struct sockaddr_un *unp = (struct sockaddr_un *)sa;

                if (unp->sun_path[0] == 0)
                {
                    strcpy(str, "no pathname bound");
                }
                else
                {
                    snprintf(str, sizeof(str), "%s", unp->sun_path);
                }
                return str;
            }
#endif

#ifdef HAVE_SOCKADDR_DL_STRUCT
        case AF_LINK:
            {
                struct sockaddr_dl *sdl = (struct sockaddr_dl *)sa;

                if (sdl->sdl_nlen > 0)
                {
                    snprintf(str, sizeof(str), "%*s", sdl->sdl_nlen, &sdl->sdl_data[0]);
                }
                else
                {
                    snprintf(str, sizeof(str), "AF_LINK, index=%d", sdl->sdl_index);
                }
                return str;
            }
#endif
        default:
            snprintf(str, sizeof(str), "sock_ntop_host: unknown AF_xxx: %d, len %d", sa->sa_family, salen);
            return str;
    }

    return NULL;
}


struct ifi_info *get_ifi_info(int family, int doaliases)
{
    struct ifi_info *ifi, *ifihead, **ifipnext;
    int sockfd, len, lastlen, flags, myflags, idx = 0, hlen = 0;
    char *ptr, *buf, lastname[IFNAMSIZ], *cptr, *haddr, *sdlname;
    struct ifconf ifc;
    struct ifreq *ifr, ifrcopy;
    struct sockaddr_in *sinptr;
    struct sockaddr_in6 *sin6ptr;

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    lastlen = 0;
    len = 100 * sizeof(struct ifreq);
    while (1)
    {
        buf = (char *)malloc(len);
        ifc.ifc_len = len;
        ifc.ifc_buf = buf;
        if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0)
        {
            if (errno != EINVAL || lastlen != 0)
            {
                err_sys("ioctl erro");
            }
        }
        else
        {
            if (ifc.ifc_len == lastlen)
            {
                break;
            }
            lastlen = ifc.ifc_len;
        }
        len += 10 * sizeof(struct ifreq);
        free(buf);
    }

    ifihead     = NULL;
    ifipnext    = &ifihead;
    lastname[0] = 0;
    sdlname     = NULL;

    for (ptr = buf; ptr < buf + ifc.ifc_len; )
    {
        // printf("ptr = buf\n");
        ifr = (struct ifreq *)ptr;

#ifdef HAVE_SOCKADDR_SA_LEN
        len = max(sizeof(struct sockaddr), ifr->ifr_addr.sa_len);
#else
        switch (ifr->ifr_addr.sa_family) 
        {
#ifdef IPV6
            case AF_INET6:
                len = sizeof(struct sockaddr_in6);
                break;
#endif
            case AF_INET:
            default:
                len = sizeof(struct sockaddr);
                break;
        }
#endif
        ptr += sizeof(ifr->ifr_name) + len;

#ifdef HAVE_SOCKADDR_DL_STRUCT
        if (ifr->ifr_addr.sa_family == AF_LINK)
        {
            // printf("AF_LINK\n");
            struct sockaddr_dl *sdl = (struct sockaddr_dl *)&ifr->ifr_addr;
            sdlname = ifr->ifr_name;
            idx = dsl->sdl_index;
            haddr = sdl->sdl_data + sdl->sdl_nlen;
            hlen = sdl->sdl_alen;
        }
#endif
        // printf("family: ifr family %d, family %d\n", ifr->ifr_addr.sa_family, family);
        if (ifr->ifr_addr.sa_family != family)
        {
            continue;
        }

        myflags = 0;
        if ((cptr = strchr(ifr->ifr_name, ':')) != NULL)
        {
            *cptr = 0;
        }
        if (strncmp(lastname, ifr->ifr_name, IFNAMSIZ) == 0)
        {
            if (doaliases == 0)
            {
                continue;
            }
            myflags = IFI_ALIAS;
        }
        memcpy(lastname, ifr->ifr_name, IFNAMSIZ);
        ifrcopy = *ifr;
        ioctl(sockfd, SIOCGIFFLAGS, &ifrcopy);
        flags = ifrcopy.ifr_flags;
        printf("flags up: %d\n", flags & IFF_UP);
        printf("flags loop: %d\n", flags & IFF_LOOPBACK);
        printf("flags broad: %d\n", flags & IFF_BROADCAST);
        if ((flags & IFF_UP) == 0)
        {
            continue;
        }

        ifi = calloc(1, sizeof(struct ifi_info));
        *ifipnext   = ifi;
        ifipnext    = &ifi->ifi_next;

        ifi->ifi_flags      = flags;
        ifi->ifi_myflags    = myflags;

#if defined(SIOCGIFMTU) && defined(HAVE_STRUCT_IFREQ_IFR_MTU)
        ioctl(sockfd, SIOCGIFMTU, &ifrcopy);
        ifi->ifi_mtu = ifrcopy.ifr_mtu;
#else
        ifi->ifi_mtu = 0;
#endif
        memcpy(ifi->ifi_name, ifr->ifr_name, IFI_NAME);
        ifi->ifi_name[IFI_NAME - 1] = '\0';

        if (sdlname == NULL || strcmp(sdlname, ifr->ifr_name) != 0)
        {
            idx = hlen = 0;
        }
        ifi->ifi_index  = idx;
        ifi->ifi_hlen   = hlen;

        if (ifi->ifi_hlen > IFI_HADDR)
        {
            ifi->ifi_hlen = IFI_HADDR;
        }
        if (hlen)
        {
            memcpy(ifi->ifi_haddr, haddr, ifi->ifi_hlen);
        }

        switch (ifr->ifr_addr.sa_family)
        {
            case AF_INET:
                sinptr = (struct sockaddr_in *)&ifr->ifr_addr;
                ifi->ifi_addr = calloc(1, sizeof(struct sockaddr_in));
                memcpy(ifi->ifi_addr, sinptr, sizeof(struct sockaddr_in));
#ifdef SIOCGIFBRDADDR
                printf("ifi-brdaddr\n");
                if (flags & IFF_BROADCAST)
                {
                    ioctl(sockfd, SIOCGIFBRDADDR, &ifrcopy);
                    sinptr = (struct sockaddr_in *)&ifrcopy.ifr_broadaddr;
                    ifi->ifi_brdaddr = calloc(1, sizeof(struct sockaddr_in));
                    memcpy(ifi->ifi_brdaddr, sinptr, sizeof(struct sockaddr_in));
                }
#endif

#ifdef SIOCGIFDSTADDR
                if (flags & IFF_POINTOPOINT)
                {
                    ioctl(sockfd, SIOCGIFDSTADDR, &ifrcopy);
                    sinptr = (struct sockaddr_in *)&ifrcopy.ifr_dstaddr;
                    ifi->ifi_dstaddr = calloc(1, sizeof(struct sockaddr_in));
                    memcpy(ifi->ifi_dstaddr, sinptr, sizeof(struct sockaddr_in));
                }
#endif
                // printf("AF_INET\n");
                break;
            case AF_INET6:
                /* 待补充 */
                // printf("AF_INET6\n");
                break;

            default:
                printf("hello world\n");
                break;
        }
    }
    free(buf);
    // printf("get_ifi_info end\n");
    return ifihead;
}

void free_ifi_info(struct ifi_info *ifihead)
{
    struct ifi_info *ifi, *ifinext;
    for (ifi = ifihead; ifi != NULL; ifi = ifinext)
    {
        if (ifi->ifi_addr != NULL)
        {
            free(ifi->ifi_addr);
        }
        if (ifi->ifi_brdaddr != NULL)
        {
            free(ifi->ifi_brdaddr);
        }
        if (ifi->ifi_dstaddr != NULL)
        {
            free(ifi->ifi_dstaddr);
        }
        ifinext = ifi->ifi_next;
        free(ifi);
    }
}
