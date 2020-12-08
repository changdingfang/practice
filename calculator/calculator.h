//***************************************************************
// @file:    calculator.h
// @author:  dingfang
// @date    2019-08-22 15:32:34
//***************************************************************

#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include <string>
#include <vector>
#include <stack>
#include <queue>

class Calculator
{
public:
    Calculator();
    ~Calculator();

    int handle(std::string rawValue);

    void readFormual(std::string formualFile);

private:
    int checkCmd(const std::string &cmd);

    int pushQueue(const std::string &data);

    int calculation(void);

    void clear();

    void calPrint() const;

    inline bool isNum(std::string &val) const
    {
        const char *p = val.c_str();
        if (val.length() == 1 && !(*p <= '9' && *p >= '0'))
        {
            return false;
        }
        return true;
    }
    inline bool isOprt(std::string &val) const
    {
        const char *p = val.c_str();
        if (val.length() == 1 && (*p == '+' || *p == '-' || *p == '*' || *p == '/'))
        {
            return true;
        }
        return false;
    }
    inline bool isLeftBracket(std::string &val) const
    {
        const char *p = val.c_str();
        if (val.length() == 1 && *p == '(')
        {
            return true;
        }
        return false;
    }

    inline bool isRightBracket(std::string &val) const
    {
        const char *p = val.c_str();
        if (val.length() == 1 && *p == ')')
        {
            return true;
        }
        return false;
    }

    inline int oprtLev(std::string &oprt) const
    {
        int lev = 0;
        if (oprt == "+" || oprt == "-")
        {
            lev = 5;
        }
        else if (oprt == "*" || oprt == "/")
        {
            lev = 8;
        }
        else if (oprt == "(" || oprt == ")")
        {
        }

        return lev;
    }

    inline bool oprtCompare(std::string &oprtL, std::string &oprtR) const
    {
        return oprtLev(oprtL) >= oprtLev(oprtR);
    }

    inline double calculationVal(double lval, double rval, char oprt)
    {
        switch (oprt)
        {
            case '+': return lval + rval;
            case '-': return lval - rval;
            case '*': return lval * rval;
            case '/': return lval / rval;
        }
    }

    int processNum(std::string &element);


private:
    typedef enum class E_ERRNO : char
    {
        SUCCESS = 0,
        INPUT_ERR = 5,
    }ERRNO_E;

    typedef enum class E_PATTERN : char
    {
        NORMAL = 0,
        PROGRAMMER = 1,
    }PATTERN_E;

private:
    PATTERN_Em_parttern;

    std::stringm_rawValue;

    std::stack<std::string> m_stk;
    std::queue<std::string> m_que;

    std::stringm_results;
    double m_calRes;

    /* 拆解的每一个单元和符号 */
    std::vector<std::string>m_element;

    std::vector<std::string>m_formual;


    ERRNO_Em_errno;

    bool m_isBack = false;
};


#endif /* __CALCULATOR_H__ */


