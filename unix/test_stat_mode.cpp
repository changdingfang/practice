// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     test_stat_mode.cpp
// Author:       dingfang
// CreateDate:   2020-11-26 19:05:41
// ModifyAuthor: dingfang
// ModifyDate:   2020-11-26 19:19:21
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

/*
 * ./a.out /etc/passwd /etc /dev/log /dev/tty ./test.fifo /dev/vda /dev/stderr 
 * */

#include <sys/stat.h>

#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    struct stat buf;
    const char *ptr;

    for (int i = 1; i < argc; ++i)
    {
        cout << argv[i] << ": ";
        if (lstat(argv[i], &buf) < 0)
        {
            cerr << "lstat error!" << endl;
            continue;
        }

        if (S_ISREG(buf.st_mode))
        {
            ptr = "regular";
        }
        else if (S_ISDIR(buf.st_mode))
        {
            ptr = "directory";
        }
        else if (S_ISCHR(buf.st_mode))
        {
            ptr = "character special";
        }
        else if (S_ISBLK(buf.st_mode))
        {
            ptr = "block special";
        }
        else if (S_ISFIFO(buf.st_mode))
        {
            ptr = "fifo";
        }
        else if (S_ISLNK(buf.st_mode))
        {
            ptr = "symbolic link";
        }
        else if (S_ISSOCK(buf.st_mode))
        {
            ptr = "socket";
        }
        else
        {
            ptr = "** unknown mode **";
        }

        cout << ptr << endl;
        cout << "uid: " << buf.st_uid << endl;
        cout << "gid: " << buf.st_gid << endl;
    }

    return 0;
}
