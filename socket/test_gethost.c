// =======================================================================
//  FileName:     gethost.c
//  Author:       dingfang
//  CreateDate:   2021-03-12 08:20:37
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-03-12 08:53:27
// =======================================================================

#include "common/unp.h"

#include <netdb.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char **argv)
{
    while (--argc > 0)
    {
        char *pname = *++argv;
        struct hostent *ph = gethostbyname(pname);
        if (ph == NULL)
        {
            err_msg("gethostbyname error for host: %s: %s", pname, hstrerror(h_errno));
            return 0;
        }

        printf("name: %s\n", ph->h_name);

        for (char **pptr = ph->h_aliases; *pptr != NULL; ++pptr)
        {
            printf("alias: %s\n", *pptr);
        }

        /* 默认为IPv4 */
        for (char **pptr = ph->h_addr_list; *pptr != NULL; ++pptr)
        {
            printf("addr: %s\n", inet_ntoa(*(struct in_addr*)(*pptr)));
        }

    }

    return 0;
}
