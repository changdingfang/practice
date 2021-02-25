// =======================================================================
//  FileName:     dftest.h
//  Author:       dingfang
//  CreateDate:   2020-12-03 19:24:43
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-02-25 21:20:15
// =======================================================================

#ifndef __DFTEST_H__
#define __DFTEST_H__

#include <sys/time.h>

#include <string>
#include <functional>
#include <list>
#include <memory>

#define DFTEST_STRINGIFY_(name) #name

#define COLOR(msg, colorNum) "\033[0;" DFTEST_STRINGIFY_(colorNum) "m" msg "\033[0m"

#define RED(msg)        COLOR(msg, 31)
#define GREEN(msg)      COLOR(msg, 32)
#define YELLOW(msg)     COLOR(msg, 33)
#define BLUE(msg)       COLOR(msg, 34)


#define LOG(fmt, args...)                                           \
{                                                                   \
    printf(YELLOW("[%s](%d): ") BLUE(fmt "\n"),                     \
            dftest::Common::filename(__FILE__).c_str(),             \
            __LINE__, ##args);                                      \
}


#define EXPECT(arg1, oper, arg2)                                    \
if (!(arg1 oper arg2))                                              \
{                                                                   \
    printf("%s:%d: Failure\n",                                      \
            dftest::Common::filename(__FILE__).c_str(),             \
            __LINE__);                                              \
    printf("Expected: (%s) %s (%s), actual: %d vs %d\n",            \
            DFTEST_STRINGIFY_(arg1),                                \
            DFTEST_STRINGIFY_(arg2),                                \
            DFTEST_STRINGIFY_(oper),                                \
            arg1,                                                   \
            arg2);                                                  \
    dftest::FailedFlag = 1;                                         \
}


#define EXPECT_EQ(arg1, arg2) EXPECT(arg1, ==, arg2)
#define EXPECT_NE(arg1, arg2) EXPECT(arg1, !=, arg2)
#define EXPECT_LT(arg1, arg2) EXPECT(arg1, <,  arg2)
#define EXPECT_LE(arg1, arg2) EXPECT(arg1, <=, arg2)
#define EXPECT_GT(arg1, arg2) EXPECT(arg1, >,  arg2)
#define EXPECT_GE(arg1, arg2) EXPECT(arg1, >=, arg2)



namespace dftest
{


    class Common
    {
    public:
        static std::string filename(const std::string fn)
        {
            std::string::size_type pos = fn.find_last_of('/');
            return pos != std::string::npos ? fn.substr(pos + 1, fn.size()) : fn;
        }

        static unsigned long getCurrMs()
        {
            struct timeval t;
            ::gettimeofday(&t, nullptr);
            return t.tv_sec * 1000 + t.tv_usec / 1000;
        }
    };


    class TestInfo
    {
    public:
        TestInfo(std::string testSuiteName, std::string testName, std::function<void(void)>func)
        : testSuiteName_(testSuiteName)
          , testName_(testName)
          , func_(func)
        { }
        const std::string name() const { return testSuiteName_ + "." + testName_; }
        const std::function<void(void)> &func() const { return func_; }

    private:
        std::string testSuiteName_;
        std::string testName_;
        std::function<void(void)> func_;

    };

    static std::list< std::shared_ptr<TestInfo> > _df_tiList;
    static int FailedFlag = 0;

    static int _df_runAllTests()
    {
        printf(GREEN("[==========]\n"));
        printf(GREEN("[----------]") " %ld tests from test\n", _df_tiList.size());
        printf(GREEN("[----------]\n"));
        size_t passCnt = 0;
        unsigned long tTotal1 = Common::getCurrMs();
        for (const auto &ti :_df_tiList)
        {
            if (!ti)
            {
                printf(RED("[ RUN FAILED ]") " init TEST failed!\n");
                continue;
            }
            printf(GREEN("[ RUN      ]") " %s\n", ti->name().c_str());
            FailedFlag = 0;
            unsigned long t1 = Common::getCurrMs();
            ti->func()();
            unsigned long t2 = Common::getCurrMs();

            std::string fmt;
            if (FailedFlag)
            {
                fmt = RED("[  FAILED  ] ");
            }
            else
            {
                fmt = GREEN("[       OK ] ");
                ++passCnt;
            }
            fmt += ti->name() + " (%ld ms)\n";
            printf(fmt.c_str(), t2 - t1);
        }
        unsigned long tTotal2 = Common::getCurrMs();
        printf(GREEN("[----------]") " %ld tests from test(%ld ms total)\n\n", _df_tiList.size(), tTotal2 - tTotal1);
        printf(GREEN("[----------]\n"));
        printf(GREEN("[==========]") " %ld tests from test\n", _df_tiList.size());
        printf(GREEN("[  PASSED  ]") " %ld tests\n", passCnt);
        if (passCnt < _df_tiList.size())
        {
            printf(RED("[  FAILED  ]") " %ld tests\n", _df_tiList.size() - passCnt);
        }

        return 0;
    }


}; /* dftest namespace end */


#define RUN_ALL_TESTS() dftest::_df_runAllTests()

#define DFTEST_FUNCTION_NAME_(testSuiteName, testName) dftest_##testSuiteName##_##testName##_

#define TEST(testSuiteName, testName)                                   \
    static_assert(sizeof(DFTEST_STRINGIFY_(testSuiteName)) > 1,         \
            "test suite name must not be empty!");                      \
    static_assert(sizeof(DFTEST_STRINGIFY_(testName)) > 1,              \
            "test name must not be empty!");                            \
    void DFTEST_FUNCTION_NAME_(testSuiteName, testName)();              \
    int _df_##testSuiteName##_##testName##_ = []() -> int {             \
        dftest::_df_tiList.push_back(                                   \
                std::make_shared<dftest::TestInfo>(                     \
                    dftest::TestInfo(                                   \
                        DFTEST_STRINGIFY_(testSuiteName),               \
                        DFTEST_STRINGIFY_(testName),                    \
                        DFTEST_FUNCTION_NAME_(testSuiteName, testName)  \
                        )));                                            \
        return 0;                                                       \
    }();                                                                \
    void DFTEST_FUNCTION_NAME_(testSuiteName, testName)()



#endif /* __DFTEST_H__ */
