// =======================================================================
//  FileName:     test_get_ifi_info.c
//  Author:       dingfang
//  CreateDate:   2021-04-11 08:56:04
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-04-11 13:47:16
// =======================================================================

#include "common/unp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    struct ifi_info *ifi, *ifihead;
    struct sockaddr *sa;
    u_char *ptr;
    
    if (argc != 3)
    {
        err_quit("usage: prifinfo <inet4 | inet6> <doaliases>");
    }

    int family;
    if (strcmp(argv[1], "inet4") == 0)
    {
        family = AF_INET;
    }
    else if (strcmp(argv[1], "inet6") == 0)
    {
        family = AF_INET6;
    }
    else
    {
        err_quit("invalid <address-family>");
    }
    int doaliases = atoi(argv[2]);

    for (ifihead = ifi = get_ifi_info(family, doaliases); ifi != NULL; ifi = ifi->ifi_next)
    {
        printf("%s: ", ifi->ifi_name);
        if (ifi->ifi_index != 0)
        {
            printf(" (%d) ", ifi->ifi_index);
        }

        printf("<");
        if (ifi->ifi_flags & IFF_UP)            printf("UP ");
        if (ifi->ifi_flags & IFF_BROADCAST)     printf("BCAST ");
        if (ifi->ifi_flags & IFF_MULTICAST)     printf("MCAST ");
        if (ifi->ifi_flags & IFF_LOOPBACK)      printf("LOOP ");
        if (ifi->ifi_flags & IFF_POINTOPOINT)   printf("P2P ");
        printf(">\n");

        int i;
        if ((i = ifi->ifi_hlen) > 0)
        {
            ptr = ifi->ifi_haddr;
            do {
                printf("%s%x", (i == ifi->ifi_hlen) ? "  " : ":", *ptr++);
            } while (--i > 0);
        }

        if (ifi->ifi_mtu != 0)
        {
            printf(" MTU: %d\n", ifi->ifi_mtu);
        }
        if ((sa = ifi->ifi_brdaddr) != NULL)
        {
            printf(" IP addr: %s\n", inet_ntop(AF_INET, (void *)sa->sa_data, NULL, sizeof(sa->sa_data)));
        }

        if ((sa = ifi->ifi_dstaddr) != NULL)
        {
            printf("----\n");
        }
    }

    free_ifi_info(ifihead);

    return 0;
}

