// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     googleTest.cpp
// Author:       dingfang
// CreateDate:   2020-12-01 21:14:23
// ModifyAuthor: dingfang
// ModifyDate:   2020-12-02 08:48:29
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

/*
 * EXPECT_* 版本的断言失败时产生非致命，不会结束当前函数
 * ASSERT_* 版本断言失败时会产生致命失败，并结束当前函数
 * */

#include <gtest/gtest.h>

int add(int a, int b)
{
    return a + b;
}

TEST(test, add1)
{
    EXPECT_EQ(add(3, 2), 5);
    EXPECT_NE(add(3, 2), 4);
    EXPECT_LT(add(3, 2), 6);
    EXPECT_LE(add(3, 2), 5);
    EXPECT_GT(add(3, 2), 4);
    EXPECT_GE(add(3, 2), 5);
}

TEST(test, t2)
{
    EXPECT_STREQ("hello world", "hello world");         /* == */
    EXPECT_STRNE("hello world", "world");               /* != */
    EXPECT_STRCASEEQ("hello world", "HELLO WORLD");     /* == 忽略大小写 */
    EXPECT_STRCASENE("hello world", "world");           /* != 忽略大小写 */

    // EXPECT_STRCASENE("hello world", "HELLO WORLD");   /* error  != 忽略大小写 */
}

TEST(test, t3)
{
    ASSERT_EQ(add(3, 5), 9);
    ASSERT_EQ(add(3, 5), 8);
}

int main(void)
{
    return RUN_ALL_TESTS();
}
