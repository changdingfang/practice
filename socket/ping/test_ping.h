// =======================================================================
//  FileName:     test_ping.h
//  Author:       dingfang
//  CreateDate:   2021-04-26 08:30:08
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-04-26 08:34:53
// =======================================================================

#ifndef __TEST_PING_H__
#define __TEST_PING_H__

#include "unp.h"

#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#define BUFSIZE 1500

char sendbuf[BUFSIZE];

int     datalen;
char    *host;
int     nsent;
pid_t   pid;
int     sockfd;
int     verbose;

void init_v6();
void proc_v4(char *, ssize_t, struct msghdr *, struct timeval *);
void proc_v6(char *, ssize_t, struct msghdr *, struct timeval *);
void send_v4();
void send_v6();
void readloop();
void sig_alrm(int);
void tv_sub(struct timeval *, struct timeval *);
uint16_t in_cksum(uint16_t *addr, int len);

struct proto {
    void (*fproc)(char *, ssize_t, struct msghdr *, struct timeval *);
    void (*fsend)();
    void (*finit)();

    struct sockaddr *sasend;
    struct sockaddr *sarecv;
    socklen_t salen;
    int icmpproto;
} *pr;


#ifdef IPV6

#include <netinet/ip6.h>
#include <netinet/icmp6.h>

#endif


#endif /* __TEST_PING_H__ */
