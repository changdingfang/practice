// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     test_rename.cpp
// Author:       dingfang
// CreateDate:   2020-11-27 08:28:46
// ModifyAuthor: dingfang
// ModifyDate:   2020-11-27 08:36:28
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <stdio.h>

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "请输入两个路径或文件" << endl;
        return -1;
    }
    string path1(argv[1]);
    string path2(argv[2]);
    /* 目录重命名时使用原目录子目录为非法操作 */
    if (path1 != path2 && path2.find(path1) != string::npos)
    {
        cerr << "不能使用子目录" << endl;
        return -1;
    }

    int ret = rename(path1.c_str(), path2.c_str());
    cout << "ret: " << ret << endl;

    return 0;
}
