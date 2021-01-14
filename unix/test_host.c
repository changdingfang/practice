// =======================================================================
//  FileName:     test_host.c
//  Author:       dingfang
//  CreateDate:   2021-01-13 19:14:01
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-13 19:56:59
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

int main(void)
{
    struct hostent *p = NULL;

    sethostent(0);
    while ((p = gethostent()) != NULL)
    {
        printf("name:\t%s\n", p->h_name);
    }
    endhostent();

    printf("\n");

    setnetent(0);
    struct netent *net = NULL;
    /* struct netent *net = getnetbyaddr(); */
    while ((net = getnetent()) != NULL)
    {
        printf("net name: %s\n", net->n_name);
        printf("net type: %d\n", net->n_addrtype);
        printf("net net: %d\n", net->n_net);
    }
    endnetent();

    printf("\n");

    setservent(0);
    struct servent *serv = NULL;
    while ((serv = getservent()) != NULL)
    {
        printf("net name: %s\n", serv->s_name);
        printf("net prot: %d\n", serv->s_port);
        printf("net proto: %s\n", serv->s_proto);
        /* break; */
    }
    endservent();

    return 0;
}
