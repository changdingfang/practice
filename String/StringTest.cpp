// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     stringTest.h
// Author:       dingfang
// CreateDate:   2020-09-02 14:49:19
// ModifyAuthor: dingfang
// ModifyDate:   2020-09-20 14:00:32
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 
#include "String.h"

#include <assert.h>
#include <string.h>

#include <iostream>
#include <sstream>
#include <fstream>

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


void check(const string &s, String::sizeType size, String::sizeType capacity)
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
    assert(s.capacity() == 15);

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
    check(s, 5, 17);

    /* 
     * 这里与string不同,这里是重新构造了一个新的String, 属性都是全新的 
     * 而string里面的成员数据可能继承的是旧的
     * */
    String s2(s);
    s = "12345";
    check(s, 5, 15);
    // check(s, 5, 17); /* 如果是string, 这里应该是这样的 */

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

    s1.append(10, 'm');
    assert(!strcmp(s1.c_str(), "hello ding fangmmmmmmmmmm"));

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


void testResize()
{
    String s1("hello world");
    s1.resize(22, 'x');
    check(s1, 22, 30);

    s1.resize(20, 'x');
    check(s1, 20, 30);

    s1.resize(8, 'x');
    check(s1, 8, 30);

    s1.resize(20);
    check(s1, 20, 30);

    s1.resize(33, 'm');
    check(s1, 33, 60);

    s1.resize(45);
    check(s1, 45, 60);

    s1.resize(133);
    check(s1, 133, 133);

    try
    {
        s1.resize(s1.max_size() + 1);
        assert(false);
    }
    catch(...) { }

    cout << "resize test success" << endl;
}


void testCopy()
{
    String s1("hello world");
    char *p = new char [50];

    ::memset(p, 0x00, 50);
    assert(s1.copy(p, 5) == 5);
    assert(!strcmp(p, "hello"));

    ::memset(p, 0x00, 50);
    assert(s1.copy(p, 5, 5) == 5);
    assert(!strcmp(p, " worl"));

    ::memset(p, 0x00, 50);
    try
    {
        s1.copy(p, 5, 50);
        assert(false);
    }
    catch(...) { }

    ::memset(p, 0x00, 50);
    assert(s1.copy(p, 50, 5) == 6);
    assert(!strcmp(p, " world"));

    ::memset(p, 0x00, 50);
    assert(s1.copy(p, 50, 0) == 11);
    assert(!strcmp(p, "hello world"));

    delete [] p;

    cout << "copy test success" << endl;
}


void testSubstr()
{
    String s1("hello world");

    String s2(s1.substr());

    assert(!strcmp(s2.c_str(), "hello world"));

    s2 = s1.substr(3);
    assert(!strcmp(s2.c_str(), "lo world"));

    s2 = s1.substr(5, 50);
    assert(!strcmp(s2.c_str(), " world"));

    s2 = s1.substr(5, 6);
    assert(!strcmp(s2.c_str(), " world"));

    try
    {
        s2 = s1.substr(50, 6);
        assert(false);
    }
    catch(const char *e) { }

    cout << "substr test success" << endl;
}


void testReplace()
{
    const char *testStr1 = "hello world";
    const char *testStr2 = "ding fang";
    String s1;
    
    auto testFunc = [&testStr1, &testStr2, &s1](String::sizeType pos, String::sizeType count, const char *expectStr) 
    {
        s1 = testStr1;
        s1.replace(pos, count, testStr2, ::strlen(testStr2));
        assert(!strcmp(s1.c_str(), expectStr));
    };

    testFunc(4, 16, "hellding fang");
    testFunc(0, 0, "ding fanghello world");
    testFunc(0, 2, "ding fangllo world");
    testFunc(3, 2, "helding fang world");
    testFunc(3, 6, "helding fangld");
    testFunc(4, 7, "hellding fang");
    testFunc(11, 0, "hello worldding fang");
    testFunc(11, 11, "hello worldding fang");
    testFunc(1, 9, "hding fangd");
    testFunc(0, 11, "ding fang");
    testFunc(0, 100, "ding fang");

    try
    {
        testFunc(::strlen(testStr1) + 1, 0, "");
        assert(false);
    }
    catch(...) { }

    String s2(testStr2);
    auto testFunc2 = [&testStr1, &testStr2, &s1, &s2]
        (String::sizeType pos, String::sizeType count
         , String::sizeType pos2, String::sizeType count2
         , const char *expectStr) 
        {
            s1 = testStr1;
            s1.replace(pos, count, s2, pos2, count2);
            assert(!strcmp(s1.c_str(), expectStr));
        };

    testFunc2(4, 16, 0, s2.size(), "hellding fang");
    testFunc2(0, 0, 2, s2.size(), "ng fanghello world");
    testFunc2(0, 2, s2.size(), 0, "hello world");
    testFunc2(3, 2, 3, 2, "helg  world");
    testFunc2(3, 2, 3, 4, "helg fa world");
    testFunc2(3, 2, 2, 20, "helng fang world");
    testFunc2(3, 6, 0, s2.size() + 1, "helding fangld");
    testFunc2(4, 7, 5, 1, "hellf");
    testFunc2(11, 0, 5, 0, "hello world");
    testFunc2(11, 11, 5, 0, "hello world");

    auto testFunc3 = [&testStr1, &s1](String::sizeType pos, String::sizeType count, String::sizeType count2, const char *expectStr) 
    {
        s1 = testStr1;
        s1.replace(pos, count, count2, 'x');

        assert(!strcmp(s1.c_str(), expectStr));
    };

    testFunc3(11, 11, 5, "hello worldxxxxx");
    testFunc3(0, 11, 5, "xxxxx");
    testFunc3(0, 3, 5, "xxxxxlo world");
    testFunc3(3, 0, 8, "helxxxxxxxxlo world");
    testFunc3(3, 5, 8, "helxxxxxxxxrld");
    testFunc3(4, 2, 6, "hellxxxxxxworld");
    testFunc3(4, 20, 0, "hell");
    testFunc3(4, 20, 1, "hellx");

    cout << "replace test success" << endl;
}


void testInsert()
{
    const char *testStr1 = "hello world";
    String s1(testStr1);

    auto testFunc = [&testStr1, &s1] (String::sizeType index, String::sizeType count, const char *expectStr) 
    {
        s1 = testStr1;
        s1.insert(index, count, 'x');

        assert(!strcmp(s1.c_str(), expectStr));
    };

    testFunc(5, 2, "helloxx world");
    testFunc(5, 5, "helloxxxxx world");
    testFunc(0, 3, "xxxhello world");
    testFunc(0, 1, "xhello world");
    testFunc(11, 1, "hello worldx");

    try
    {
        testFunc(12, 5, "");
        assert(false);
    }
    catch(...) { }

    auto testFunc2 = [&testStr1, &s1] (String::sizeType index, String::sizeType count, const char * data, const char *expectStr) 
    {
        s1 = testStr1;
        if (count == 0)
        {
            s1.insert(index, data);
        }
        else
        {
            s1.insert(index, data, count);
        }

        assert(!strcmp(s1.c_str(), expectStr));
    };

    testFunc2(3, 0, "test", "heltestlo world");
    testFunc2(11, 0, "test", "hello worldtest");
    testFunc2(0, 0, "test", "testhello world");
    testFunc2(8, 0, "test", "hello wotestrld");
    try
    {
        testFunc2(12, 0, "test", "heltestlo world");
        assert(false);
    }
    catch(...) { }

    testFunc2(3, 3, "test", "helteslo world");
    testFunc2(11, 1, "test", "hello worldt");
    testFunc2(0, 2, "test", "tehello world");
    testFunc2(8, 2, "test", "hello woterld");

    String s2("ding fang");
    auto testFunc3 = [&testStr1, &s1, &s2](String::sizeType index, String::sizeType index2, String::sizeType count, const char *expectStr) 
    {
        s1 = testStr1;
        if (index2 == 0)
        {
            s1.insert(index, s2);
        }
        else
        {
            s1.insert(index, s2, index2, count);
        }

        assert(!strcmp(s1.c_str(), expectStr));
    };

    testFunc3(8, 0, 0, "hello woding fangrld");
    testFunc3(2, 0, 0, "heding fangllo world");
    testFunc3(0, 0, 0, "ding fanghello world");
    testFunc3(11, 0, 0, "hello worldding fang");

    testFunc3(8, 2, 0, "hello world");
    testFunc3(8, 2, 2, "hello wongrld");
    testFunc3(2, 2, 2, "hengllo world");
    testFunc3(0, 3, 4, "g fahello world");
    testFunc3(11, 2, 5, "hello worldng fa");

    cout << "insert test success" << endl;
}


void testFind()
{
    /*                   10        20*/
    /*         012345678901234567890*/
    String s1("hello world ding fang");

    assert(s1.find("ding") == 12);
    assert(s1.find(" ") == 5);
    assert(s1.find("world") == 6);
    assert(s1.find("dingx") == String::npos);
    assert(s1.find(" d") == 11);
    assert(s1.find('n') == 14);
    assert(s1.find(' ', 6) == 11);

    String s2("d ");
    assert(s1.find(s2) == 10);

    assert(s1.find(" ", 6) == 11);
    assert(s1.find("ding test", 6, 5) == 12);
    s2 = s1 + "hello";
    assert(s1.find(s2, 0) == String::npos);


    assert(s1.rfind(" ", 0) == 16);
    assert(s1.rfind("l", 0) == 9);
    assert(s1.rfind("ng", 0) == 14);
    assert(s1.rfind("dingx", 0) == String::npos);

    cout << "find test success" << endl;
}


void testFindFirst()
{
    String s1("abcdefghijklmnopqrstuvwxyz0123456789");

    assert(s1.find_first_of("01i", 0, 3) == 8);
    assert(s1.find_first_of("01", 0, 2) == 26);
    assert(s1.find_first_of("-^!@", 0, 4) == String::npos);
    assert(s1.find_first_of("-9", 0, 2) == 35);
    assert(s1.find_first_of("+-9*&#", 0, 6) == 35);
    assert(s1.find_first_of("fg9", 15, 3) == 35);
    assert(s1.find_first_of("+kj-9*&#", 20, 8) == 35);
    assert(s1.find_first_of("+kj-9*&#", String::npos, 8) == String::npos);

    assert(s1.find_first_of("+-0*&#") == 26);
    assert(s1.find_first_of("+-0*&#", 1) == 26);
    assert(s1.find_first_of("+-0*&#", 26) == 26);
    assert(s1.find_first_of("+-0*&#", 27) == String::npos);

    assert(s1.find_first_of('c', 27) == String::npos);
    assert(s1.find_first_of('c', 1) == 2);
    assert(s1.find_first_of('c') == 2);
    assert(s1.find_first_of('9') == 35);


    assert(s1.find_first_not_of("abcdemnfg9") == 7);
    assert(s1.find_first_not_of("demnfabcg9") == 7);
    assert(s1.find_first_not_of("demnfabcg9", 12) == 14);
    assert(s1.find_first_not_of("demnfabcg9", 12, 4) == 14);
    assert(s1.find_first_not_of("demnfabcg9", 12, 3) == 13);
    assert(s1.find_first_not_of("demnfabcg9", 12, 2) == 12);
    assert(s1.find_first_not_of("demnfabcg9", 0, 5) == 0);
    assert(s1.find_first_not_of("demnfabcg9", String::npos, 5) == String::npos);
    assert(s1.find_first_not_of('d') == 0);
    assert(s1.find_first_not_of('d', 3) == 4);


    assert(s1.find_last_of("01i", 0, 3) == String::npos);
    assert(s1.find_last_of("01", String::npos, 2) == 27);
    assert(s1.find_last_of("-^!@", String::npos, 4) == String::npos);
    assert(s1.find_last_of("-9", String::npos, 2) == 35);
    assert(s1.find_last_of("+-9*&#", String::npos, 6) == 35);
    assert(s1.find_last_of("fg9", 15, 3) == 6);
    assert(s1.find_last_of("+kj-9*&#", 20, 8) == 10);

    assert(s1.find_last_of("+-0*&#") == 26);
    assert(s1.find_last_of("+-0*&#", 1) == String::npos);
    assert(s1.find_last_of("+-0*&#", 26) == 26);
    assert(s1.find_last_of("+-0*&#", 27) == 26);

    assert(s1.find_last_of('c', 27) == 2);
    assert(s1.find_last_of('c', 1) == String::npos);
    assert(s1.find_last_of('c') == 2);
    assert(s1.find_last_of('9') == 35);


    assert(s1.find_last_not_of("demnfabcg9") == 34);
    assert(s1.find_last_not_of("demnfabcg9", 12) == 11);
    assert(s1.find_last_not_of("demnfabcg9", 12, 4) == 11);
    assert(s1.find_last_not_of("demnfabcg9", 12, 3) == 11);
    assert(s1.find_last_not_of("demnfabcg9", 12, 2) == 12);
    assert(s1.find_last_not_of("demnfabcg9", 0, 5) == 0);
    assert(s1.find_last_not_of('d') == 35);
    assert(s1.find_last_not_of('d', 3) == 2);

    cout << "find first test success" << endl;
}


void testIstrem()
{
    String s;

    cout << "等待输入: " << endl;
    cin >> s;
    /* input: hello world       output: hello */
    /* input: hello    world    output: hello (这里输入的空白是\t)*/
    /* input: helloworld        output: helloworld */
    cout << s << endl;

    cout << "istream test success" << endl;
}


void testGetline()
{
    String s;

    cout << "等待输入: " << endl;
    df::getline(cin, s);
    cout << s << endl;

    std::istringstream input;
    input.str("1\n2\n3\n4\n5\n6\n7\n");
    for (String line; df::getline(input, line); )
    {
        cout << "line: " << line << endl;
    }

    ifstream ifs("./test.txt");
    for (String s2; df::getline(ifs, s2); )
    {
        cout << "file line: " << s2 << endl;
    }

    cout << "getline test success" << endl;
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
#if 1
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
    testSwap();
    testResize();
    testCopy();
    testSubstr();
    testReplace();
    testInsert();
    testFind();
    testFindFirst();
    // testIstrem();
    // testGetline();
#endif

    // testBug();

    return 0;
}
