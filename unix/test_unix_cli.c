// =======================================================================
//  FileName:     test_unix_cli.c
//  Author:       dingfang
//  CreateDate:   2021-01-16 10:06:14
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-16 10:14:55
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

#define MAXMSZ 512

struct mymesg
{
    long mtype;
    char mtext[MAXMSZ];
};

int main(int argc, char **argv)
{
    key_t key;
    long qid;
    size_t nbytes;
    struct mymesg m;

    if (argc != 3)
    {
        printf("usage: sendmsg KEY message\n");
        exit(-1);
    }

    key = strtol(argv[1], NULL, 0);
    if ((qid = msgget(key, 0)) < 0)
    {
        printf("msgget failed!, key: %d\n", key);
        exit(-1);
    }

    memset(&m, 0x00, sizeof(m));
    strncpy(m.mtext, argv[2], MAXMSZ - 1);
    nbytes = strlen(m.mtext);
    m.mtype = 1;
    if (msgsnd(qid, &m, nbytes, 0) < 0)
    {
        printf("msgsnd failed!\n");
        exit(-1);
    }

    return 0;
}
