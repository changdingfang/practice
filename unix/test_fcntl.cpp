// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     test_fcntl.cpp
// Author:       dingfang
// CreateDate:   2020-11-25 19:39:48
// ModifyAuthor: dingfang
// ModifyDate:   2020-11-25 20:14:11
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

/*
 * ./a.out 0 < /dev/tty
 * ./a.out 1 > temp.foo
 * ./a.out 2 2 >> temp.foo
 * ./a.out 5 5<>temp.foo
 * */
#include <fcntl.h>

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
    int val;
    if (argc != 2)
    {
        cerr << "usage: a.out < descriptor#>" << endl;
        return -1;
    }

    if ((val = fcntl(stoi(argv[1]), F_GETFL, 0)) < 0)
    {
        cerr << "fcntl error for fd " << stoi(argv[1]) << endl;
        return -1;
    }

    switch (val & O_ACCMODE)
    {
        case O_RDONLY:  cout << "read only" << endl;     break;
        case O_WRONLY:  cout << "write only" << endl;    break;
        case O_RDWR:    cout << "read write" << endl;    break;
        default:        cout << "unknow access mode" << endl;
    }

    if (val & O_APPEND)
    {
        cout << "append" << endl;
    }
    if (val & O_NONBLOCK)
    {
        cout << "nonblocking" << endl;
    }
    if (val & O_SYNC)
    {
        cout << "synchronous writes" << endl;
    }

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
    if (val & O_FSYNC)
    {
        cout << "synchronous writes" << endl;
    }
#endif

    return 0;
}
