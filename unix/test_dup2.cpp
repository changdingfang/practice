// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     test_dup2.cpp
// Author:       dingfang
// CreateDate:   2020-11-25 21:26:34
// ModifyAuthor: dingfang
// ModifyDate:   2020-11-25 21:36:45
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
#include <string>

using namespace std;

int main(void)
{
    const char *path = "hello.txt";
    int fd = open(path, O_RDWR | O_CREAT, 0777);
    // fd = 3;

    cout << "fd: " << fd << endl;
    dup2(fd, 0);
    dup2(fd, 2);
    dup2(fd, 1);

    close(fd);

    const char *buf = "hello world\n";
    write(0, buf, strlen(buf));
    write(1, buf, strlen(buf));
    write(2, buf, strlen(buf));
    write(fd, buf, strlen(buf));

    return 0;
}
