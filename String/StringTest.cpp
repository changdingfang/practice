 //***************************************************************
 // @file:    stringTest.cpp
 // @author:  dingfang
 // @date    2020-09-02 14:49:19
 //***************************************************************
 
#include "String.h"

#include <assert.h>
#include <string.h>

#include <iostream>

using namespace std;
using namespace df;

void testRefCount(void)
{
    String s1, s2("hello world");

    assert(s1.refCount() == 1);
    assert(s2.refCount() == 1);

    s1 = s2;

    assert(s1.refCount() == 2);
    assert(s2.refCount() == 2);

    String s3(s2);

    assert(s1.refCount() == 3);
    assert(s2.refCount() == 3);
    assert(s3.refCount() == 3);

    {
        String s4(s2);

        assert(s1.refCount() == 4);
        assert(s2.refCount() == 4);
        assert(s3.refCount() == 4);
        assert(s4.refCount() == 4);
    }

    assert(s1.refCount() == 3);
    assert(s2.refCount() == 3);
    assert(s3.refCount() == 3);

    s2 = "hello world";

    assert(s1.refCount() == 2);
    assert(s2.refCount() == 1);
    assert(s3.refCount() == 2);

    s3 = s2;

    assert(s1.refCount() == 1);
    assert(s2.refCount() == 2);
    assert(s3.refCount() == 2);

    cout << "refCount test success" << endl;
}


void testIndex()
{
    const char *testString = "hello world";
    String s1(testString);
    String s2(s1);

    assert(s1[0] == 'h');

    char *p = &s1[1];
    *p = 'x';

    assert(s1[1] == 'x');
    assert(!strcmp(s1.c_str(), "hxllo world"));
    assert(!strcmp(s2.c_str(), testString));

    String s3(s1);
    char *p1 = &s1[1];
    *p1 = 'm';

    assert(s1[1] == 'm');
    assert(s2[1] == 'e');
    assert(s3[1] == 'x');
    assert(!strcmp(s1.c_str(), "hmllo world"));
    assert(!strcmp(s2.c_str(), testString));
    assert(!strcmp(s3.c_str(), "hxllo world"));

    String str1, str2, str3(testString);

    str2 = str3;
    char *pc = &str3[1];

    str1 = str3;

    assert(!strcmp(str1.c_str(), testString));
    assert(!strcmp(str2.c_str(), testString));
    assert(!strcmp(str3.c_str(), testString));

    for (int i = 0; i < strlen(testString); ++i)
    {
        assert(testString[i] == str1[i]);
    }

    cout << "index test success" << endl;
}


void testFront()
{
    String s1("hello wrold");

    char &c1 = s1.front();

    assert(c1 == 'h');
    c1 = 'x';
    assert(c1 == s1[0]);

    String s2;

    char &c2 = s2.front();
    assert(c2 == '\0');

    c2 = 'm';
    assert(c2 == s2[0]);
    assert(s2.c_str()[0] == 'm');

    cout << "front test success" << endl;
}


void testBack()
{
    String s1("hello wrold");

    char &c1 = s1.back();

    assert(c1 == s1[strlen(s1.c_str()) - 1]);

    c1 = 'x';
    assert(c1 == s1[strlen(s1.c_str()) - 1]);

    String s2;

    char &c2 = s2.back();

    cout << "back test success" << endl;
}


void testAt()
{
    String s1("hello wrold");

    char &c1 = s1.at(2);

    assert(s1.at(2) == 'l');
    assert(c1 == 'l');

    c1 = 'x';
    assert(s1.at(2) == c1);

    String s2;
    try
    {
        char &c2 = s2.at(0);
        assert(false);
    }
    catch(const char *e)
    { }

    cout << "at test success" << endl;
}


void check(const String &s, String::sizeType size, String::sizeType capacity)
{
#if 0
    cout << "length: " << s.length() << endl;
    cout << "__size: " << s.size() << endl;
    cout << "capacity: " << s.capacity() << endl;
    cout << "- - - - - " << endl;
#endif
    assert(s.length() == size);
    assert(s.size() == size);
    assert(s.capacity() == capacity);
}


void testReserve()
{
    String s;
    check(s, 0, 15);
    assert(s.capacity());

    s = "hello world";
    check(s, 11, 15);
    s.reserve(8);
    check(s, 11, 15);

    s.reserve(16);
    check(s, 11, 30);

    s.reserve(70);
    check(s, 11, 70);

    s.reserve(34);
    check(s, 11, 34);

    s.reserve(22);
    check(s, 11, 22);

    s.reserve(0);
    check(s, 11, 15);

    s = "hello world1234";
    check(s, 15, 15);

    s = "hello world123456";
    check(s, 17, 30);

    s.reserve(10);
    check(s, 17, 17);

    s.reserve(22);
    check(s, 17, 34);

    s.reserve(36);
    check(s, 17, 68);

    s.shrink_to_fit();
    check(s, 17, 17);

    s = "12345";
    check(s, 5, 15);

    cout << "reserve test success" << endl;
}


void testClear()
{
    String s1("hello world");

    String s2(s1);
    assert(s1.refCount() == 2);
    assert(s2.refCount() == 2);

    s1.clear();
    assert(s1.refCount() == 1);
    assert(s2.refCount() == 1);
    assert(s1[0] == '\0');
    assert(!strcmp(s2.c_str(), "hello world"));

    cout << "clear test success" << endl;
}


void testAppend()
{
    String s1("hello world"), s2("ding fang");

    check(s1, 11, 15);
    s1 += s2;

    check(s1, 20, 30);
    assert(!strcmp(s1.c_str(), "hello worldding fang"));

    s1 += "abcd";
    assert(!strcmp(s1.c_str(), "hello worldding fangabcd"));

    s1 += 'x';
    assert(!strcmp(s1.c_str(), "hello worldding fangabcdx"));

    s1.clear();
    assert(!strcmp(s1.c_str(), ""));

    s1.append("hello ");
    assert(!strcmp(s1.c_str(), "hello "));

    s1.append(s2);
    assert(!strcmp(s1.c_str(), "hello ding fang"));

    s1.append('m');
    assert(!strcmp(s1.c_str(), "hello ding fangm"));

    cout << "append test success" << endl;
}


void testCompare()
{
    String s1("hello");
    String s2("hello world");

    assert(s1.compare(s2));
    assert(s2.compare(s1));
    assert(s1.compare("hello world"));
    assert(!s2.compare("hello world"));

    String s3("llo ");
    assert(s1.compare(2, 4, s3));
    assert(!s2.compare(2, 4, s3));

    assert(!s1.compare(1, 1, "e"));
    assert(!s2.compare(4, 1, "o"));

    assert(!s1.compare(0, s1.size(), "xzbhellof", 3, 5));
    assert(!s2.compare(6, 5, "fjworld", 2, 5));

    assert(!(s2 == s1));
    assert((s2 == "hello world"));
    assert(!(s1 == "hello world"));
    assert(("hello world" == s2));

    cout << "compare test success" << endl;
}


void testErase()
{
    String s1("hello world");

    s1.pop_back();
    assert(s1 == "hello worl");
    s1.pop_back();
    assert(s1 == "hello wor");
    s1.pop_back();
    assert(s1 == "hello wo");

    String s2("0123456789"), s3, s4, s5, s6;
    s6 = s5 = s4 = s3 = s1 = s2;
    s2.erase();
    assert(s2 == "");

    s1.erase(5);
    assert(s1 == "01234");
    s3.erase(4, 100);
    assert(s3 == "0123");

    s4.erase(4, 6);
    assert(s4 == "0123");

    s5.erase(4, 2);
    assert(s5 == "01236789");

    s6.erase(0, 3);
    assert(s6 == "3456789");

    try
    {
        s5.erase(s5.size() + 1);
        assert(false);
    }
    catch(...) { }

    cout << "erase test success" << endl;
}


void testSwap()
{
    String s1("hello world"), s2("ding fang"), s3;

    s1.swap(s2);
    assert(s1 == "ding fang");
    assert(s2 == "hello world");
    s1.swap(s3);
    assert(s1 == "");
    assert(s3 == "ding fang");

    cout << "swap test success" << endl;
}


void testBug()
{
    String s1("hello world");

    /* 这是一个问题,p输出了旧值 */
    const char *p = s1.data();
    String s2(s1);
    s1[0] = 'x';
    cout << s1.data() << endl;
    cout << p << endl;

    // const String s3("hello world");
    // cout << "s3: " << s3 << endl;
    // cout << "s3[2]: " << s3[2] << endl;
}


int main(void)
{
#if 0
    testRefCount();
    testIndex();
    testFront();
    testBack();
    testAt();
    testReserve();
    testClear();
    testAppend();
    testCompare();
    testErase();
#endif
    testSwap();


    // testBug();

    return 0;
}
