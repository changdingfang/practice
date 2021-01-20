// =======================================================================
//  FileName:     tcattr.c
//  Author:       dingfang
//  CreateDate:   2021-01-18 22:04:09
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-18 22:07:18
// =======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int main(void)
{
    struct termios term;

    if (tcgetattr(STDIN_FILENO, &term))
    {
        printf("tcgetattr failed!\n");
        exit(-1);
    }

    switch (term.c_cflag & CSIZE)
    {
        case CS5: printf("5 bits/byte\n"); break;
        case CS6: printf("6 bits/byte\n"); break;
        case CS7: printf("7 bits/byte\n"); break;
        case CS8: printf("8 bits/byte\n"); break;
        default: printf("unknown bits/byte\n");
    }

    term.c_cflag &= ~CSIZE;
    term.c_cflag |= CS8;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0)
    {
        printf("tcsetattr failed!\n");
        exit(-1);
    }

    return 0;
}
