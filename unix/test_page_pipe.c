// =======================================================================
//  FileName:     test_page_pipe.c
//  Author:       dingfang
//  CreateDate:   2021-01-04 20:16:28
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-04 20:43:10
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAXLINE 2048
#define DEF_PAGER "/bin/more"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("usage: %s <pathname>", argv[0]);
        return -1;
    }

    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        printf("can't open %s\n", argv[1]);
        exit(-1);
    }

    int fd[2];
    if (pipe(fd) < 0)
    {
        printf("pipe failed!\n");
        exit(-1);
    }

    char line[MAXLINE];
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        printf("fork failed!\n");
        exit(-1);
    }
    else if (pid > 0)
    {
        close(fd[0]);
        while (fgets(line, MAXLINE, fp) != NULL)
        {
            int n = strlen(line);
            if (write(fd[1], line, n) != n)
            {
                printf("write failed!\n");
                exit(-1);
            }
        }

        if (ferror(fp))
        {
            printf("ferror failed!\n");
            exit(-1);
        }
        close(fd[1]);

        if (waitpid(pid, NULL, 0) < 0)
        {
            printf("waitpid failed!\n");
            exit(-1);
        }
    }
    else
    {
        close(fd[1]);
        /* 保证管道读端与标准输出不是同一个描述符 */
        if (fd[0] != STDIN_FILENO)
        {
            /* 使标准输入成为管道的读端 */
            if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
            {
                printf("dup2 failed!\n");
                exit(-1);
            }
            close(fd[0]);
        }

        /* 分页程序 more */
        char *pager, *argv0;
        if ((pager = getenv("PAGER")) == NULL)
        {
            pager = DEF_PAGER;
        }

        if ((argv0 = strrchr(pager, '/')) != NULL)
        {
            argv0++;
        }
        else
        {
            argv0 = pager;
        }

        if (execl(pager, argv0, (char *)0) < 0)
        {
            printf("execl failed!\n");
            exit(-1);
        }
    }

    return 0;
}
