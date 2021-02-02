// =======================================================================
//  FileName:     newNonQuick.cpp
//  Author:       dingfang
//  CreateDate:   2021-02-02 08:41:02
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-02-02 20:14:23
// =======================================================================

#include <stdlib.h>
#include <sys/time.h>

#include <iostream>
#include <stack>
#include <vector>


using namespace std;

const int LEN = 100000;


unsigned long calTime(struct timeval &v1, struct timeval &v2)
{
    return ((v2.tv_sec - v1.tv_sec) * 1000 + (v2.tv_usec - v1.tv_usec) / 1000);
}


void check(const vector<int> &v)
{
    for (int i = 1; i < v.size(); ++i)
    {
        if (v[i - 1] > v[i])
        {
            cout << "quick sort error!\n";
            return ;
        }
    }

    /* cout << "quick sort successful\n"; */
    return ;
}


int quickSort(vector<int> &v, int left, int right)
{
    int key = v[left];
    int start = left;

    while (left < right)
    {
        while (left < right && v[left] <= key)
        {
            ++left;
        }

        while (left < right && v[right] >= key)
        {
            --right;
        }

        v[right] = key;
    }

    v[left] = key;

    return left;
}


void quick(vector<int> &vec)
{
    stack<int> idxStk;

    idxStk.push(0);
    idxStk.push(vec.size() - 1);

    while (!idxStk.empty())
    {
        int right = idxStk.top();
        idxStk.pop();
        int left  = idxStk.top();
        idxStk.pop();
        int pos = quickSort(vec, left, right);

        if (right > pos + 1)
        {
            idxStk.push(pos + 1);
            idxStk.push(right);
        }

        if (pos - 1 > left)
        {
            idxStk.push(left);
            idxStk.push(pos - 1);
        }
    }
}


int main(void)
{
    srand(time(nullptr));
    vector<int> v(LEN);

    int count = 10;

    do {
        for (int i = 0; i < LEN; ++i)
        {
            v[i] = rand() % 10000;
        }

        struct timeval v1;
        gettimeofday(&v1, nullptr);
        quick(v);
        struct timeval v2;
        gettimeofday(&v2, nullptr);

        cout << "count: " << count << "  size: " << v.size() << endl;
        cout << "use time: " << calTime(v1, v2) << "ms\n";

        check(v);
    } while (--count);

    return 0;
}
