// =======================================================================
//  FileName:     test_vim.c
//  Author:       dingfang
//  CreateDate:   2020-12-30 19:13:23
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-30 19:15:53
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        fprintf(stderr, "usage: input filename\n");
        exit(0);
    }
    char *p = (char *)malloc(strlen(argv[1]) + 5);
    strcpy(p, "vim ");
    strcpy(p + 4, argv[1]);
    system(p);

    return 0;
}
