// =======================================================================
//  FileName:     test_windows.c
//  Author:       dingfang
//  CreateDate:   2021-01-25 20:41:39
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-25 20:46:33
// =======================================================================

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>

static void pr_winsize(int fd)
{
    struct winsize size;
    if (ioctl(fd, TIOCGWINSZ, (char *)&size) < 0)
    {
        printf("ioctl failed!\n");
        exit(-1);
    }

    printf("%d rows, %d columns\n", size.ws_row, size.ws_col);
}

static void sig_winch(int signo)
{
    printf("SIGWINCH received\n");
    pr_winsize(STDIN_FILENO);
}

int main(void)
{
    if (isatty(STDIN_FILENO) == 0)
    {
        exit(1);
    }

    if (signal(SIGWINCH, sig_winch) == SIG_ERR)
    {
        printf("signal failed!\n");
        exit(-1);
    }

    pr_winsize(STDIN_FILENO);

    for (;;)
    {
        pause();
    }

    return 0;
}
