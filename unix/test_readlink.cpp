// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     test_readlink.cpp
// Author:       dingfang
// CreateDate:   2020-11-27 19:00:57
// ModifyAuthor: dingfang
// ModifyDate:   2020-11-27 20:36:11
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <unistd.h>

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cerr << "请输入一个符号连接" << endl;
        return -1;
    }
    const int size = 50;
    char buf[size] = { 0 };
    readlink(argv[1], buf, size);

    cout << "buf: [" << buf << "]" << endl;

    return 0;
}
