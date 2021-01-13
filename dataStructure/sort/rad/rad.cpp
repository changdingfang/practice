// =======================================================================
//  FileName:     rad.cpp
//  Author:       dingfang
//  CreateDate:   2021-01-13 19:11:20
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-13 19:38:25
// =======================================================================

#include <stdlib.h>

#include <iostream>
#include <vector>

using namespace std;

void print(const vector<int> &v)
{
    for (const auto &val : v)
    {
        printf("%5d", val);
    }
    cout << endl;
}


bool checkSortResult(const vector<int> &sortVec)
{
    for (int i = 1; i < sortVec.size(); ++i)
    {
        if (sortVec[i - 1] > sortVec[i])
        {
            cout << "sort error!" << endl;
            return false;
        }
    }

    cout << "sort successful!" << endl;

    return true;
}


void generateNums(vector<int> &rawNums)
{
    for (auto & val :rawNums)
    {
        val = rand() % 1000000;
    }
}


int getNumLen(int num)
{
    int numCount = 1;
    while ((num = num / 10))
    {
        ++numCount;
    }

    return numCount;
}


void rad(vector<int> &rawNums)
{
    int maxNum = 0x80000000;

    vector<vector<int>> v(10);
    for (auto val : rawNums)
    {
        int idx = val % 10;
        v[idx].push_back(val);
        if (val > maxNum)
        {
            maxNum = val;
        }
    }

    int i = 0;
    for (const auto &v1 : v)
    {
        for (const auto &val :v1)
        {
            rawNums[i++] = val;
        }
    }

    int numCount = getNumLen(maxNum);

    for (int i = 1, div = 10; i < numCount; ++i, div *= 10)
    {
        vector<vector<int>> v(10);
        for (auto val : rawNums)
        {
            int idx = (val / div) % 10;
            v[idx].push_back(val);
        }
        int j = 0;
        for (const auto &v1 : v)
        {
            for (const auto &val : v1)
            {
                rawNums[j++] = val;
            }
        }
    }
}


int main(void)
{
    srand(time(nullptr));
    vector<int> rawNums(1 * 10000);
    generateNums(rawNums);

    /* cout << "raw: \n"; */
    /* print(rawNums); */

    rad(rawNums);

    /* cout << "sort after: \n"; */
    /* print(rawNums); */

    checkSortResult(rawNums);

    return 0;
}
