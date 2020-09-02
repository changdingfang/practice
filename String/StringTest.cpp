 //***************************************************************
 // @file:    stringTest.cpp
 // @author:  dingfang
 // @date    2020-09-02 14:49:19
 //***************************************************************
 
#include "String.h"

#include <iostream>

using namespace std;
using namespace df;


void testRefCount(void)
{
    String s1, s2("hello world");

    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;

    cout << "s1: " << s1.refCount() << endl;;
    cout << "s2: " << s2.refCount() << endl;

    s1 = s2;

    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;
    cout << "s1: " << s1.refCount() << endl;
    cout << "s2: " << s2.refCount() << endl;

    String s3(s2);

    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;
    cout << "s3: " << s3 << endl;
    cout << "s1: " << s1.refCount() << endl;
    cout << "s2: " << s2.refCount() << endl;
    cout << "s3: " << s3.refCount() << endl;

    {
        String s4(s2);

        cout << "s1: " << s1 << endl;
        cout << "s2: " << s2 << endl;
        cout << "s3: " << s3 << endl;
        cout << "s4: " << s3 << endl;
        cout << "s1: " << s1.refCount() << endl;
        cout << "s2: " << s2.refCount() << endl;
        cout << "s3: " << s3.refCount() << endl;
        cout << "s4: " << s4.refCount() << endl;
    }

    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;
    cout << "s3: " << s3 << endl;
    cout << "s1: " << s1.refCount() << endl;
    cout << "s2: " << s2.refCount() << endl;
    cout << "s3: " << s3.refCount() << endl;

    s2 = "hello world";

    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;
    cout << "s3: " << s3 << endl;
    cout << "s1: " << s1.refCount() << endl;
    cout << "s2: " << s2.refCount() << endl;
    cout << "s3: " << s3.refCount() << endl;

    s3 = s2;
    cout << "s1: " << s1.refCount() << endl;
    cout << "s2: " << s2.refCount() << endl;
    cout << "s3: " << s3.refCount() << endl;
}

void testIndex()
{
    String s1("hello world");

    String s2 = s1;

    cout << "c: " << s1[0] << endl;
    char *p = &s1[1];
    *p = 'x';

    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;

    String s3(s1);
    char *p1 = &s1[1];
    *p1 = 'm';
    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;
    cout << "s3: " << s3 << endl;
}

void testFront()
{
    String s1("hello wrold");

    char &c1 = s1.front();

    cout << "c1: " << c1 << endl;

    c1 = 'x';
    cout << "s1: " << s1 << endl;

    String s2;

    char &c2 = s2.front();

    cout << "c2: " << c2 << endl;
    c2 = 'm';
    cout << "s2: " << s2 << endl;
}

void testBack()
{
    String s1("hello wrold");

    char &c1 = s1.back();

    cout << "c1: " << c1 << endl;

    c1 = 'x';
    cout << "s1: " << s1 << endl;

    String s2;

    char &c2 = s2.front();

    cout << "c2: " << c2 << endl;
    c2 = 'm';
    cout << "s2: " << s2 << endl;
}

void testAt()
{
    String s1("hello wrold");

    char &c1 = s1.at(2);

    cout << "c1: " << c1 << endl;

    c1 = 'x';
    cout << "s1: " << s1 << endl;

    String s2;
    try
    {
        char &c2 = s2.at(0);
    }
    catch(const char *e)
    {
        cout << "err: " << e << endl;
    }
}

int main(void)
{
    // testRefCount();
    // testIndex();
    // testFront();
    // testBack();
    testAt();

    return 0;
}
