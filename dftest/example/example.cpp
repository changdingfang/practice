// =======================================================================
//  FileName:     example.cpp
//  Author:       dingfang
//  CreateDate:   2020-12-03 19:00:03
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-02-25 21:19:34
// =======================================================================

#include <dftest/dftest.h>
#include <unistd.h>

#include <iostream>
#include <string>

using namespace std;

TEST(hello, dingfang)
{
    EXPECT_EQ(1, 1);
#if 0
    EXPECT_EQ(2, 2);
    EXPECT_EQ(2, 2);
    EXPECT_EQ(3, 3);
    EXPECT_EQ(1, 1);
#endif
    usleep(100 * 2000);
}

#if 0
TEST(hello, dingfang2)
{
    EXPECT_EQ(1, 1);
    EXPECT_EQ(1, 1);
    EXPECT_EQ(1, 1);
    EXPECT_EQ(1, 1);
    EXPECT_EQ(1, 1);
    EXPECT_EQ(1, 1);
    usleep(10 * 2000);
}
#endif

#if 0

TEST(hello, dingfang3)
{
    EXPECT_EQ(1, 1);
    EXPECT_EQ(1, 1);
    EXPECT_EQ(1, 1);
    EXPECT_EQ(1, 1);
}


TEST(hello, dingfang4)
{
    EXPECT_EQ(1, 1);
    EXPECT_EQ(1, 0);
    EXPECT_EQ(1, 1);
}

TEST(hello, dingfang5)
{
    EXPECT_EQ(1, 0);
    EXPECT_EQ(1, 1);
    EXPECT_EQ(1, 2);
    EXPECT_EQ(1, 1);
}
#endif

int main(void)
{

    return RUN_ALL_TESTS();
}
