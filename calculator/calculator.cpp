//***************************************************************
// @file:    calculator.cpp
// @author:  dingfang
// @date    2019-08-23 08:36:26
//***************************************************************

#include "calculator.h"
#include <unistd.h>

#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stack;
using std::to_string;
using std::ifstream;
using std::getline;

Calculator::Calculator()
    : m_parttern(PATTERN_E::NORMAL)
    , m_rawValue("")
    , m_stk()
      , m_que()
    , m_results("")
    , m_element()
      , m_errno(ERRNO_E::SUCCESS)
{
}

Calculator::~Calculator()
{
}

int Calculator::handle(std::string rawValue)
{
    this->checkCmd(rawValue);
    this->pushQueue(rawValue);
    this->calculation();

    this->calPrint();

    return 0;
}

int Calculator::checkCmd(const std::string &cmd)
{
    if (cmd == "AC")
    {
        this->clear();
    }
    if (cmd == "C")
    {
        this->clear();
    }
    if (cmd == "NORMAL")
    {
    }

    return 0;
}

int Calculator::pushQueue(const std::string &data)
{
    cout << "data: " << data << endl;

    string element("");

    const char *pPos = data.c_str();
    const char *const pEnd = pPos + data.length();

    while (pPos != pEnd)
    {
        if ((*pPos <= '9' && *pPos >= '0') || *pPos == '.')
        {
            element.append(1, *pPos);
        }
        else
        {
            if (!element.empty())
            {
                m_que.push(element);
                element.clear();
            }
            if (!isspace(*pPos))
            {
#if 0
                if (!m_que.empty())
                {
                    string tmp = m_que.back();
                    const char *pTmp = tmp.c_str();
                    if (tmp.length() == 1 && !(*pTmp <= '9' && *pTmp >= '0'))
                    {
                        m_que.pop();
                    }
                }
#endif
                element.append(1, *pPos);
                m_que.push(element);
                element.clear();
            }
        }
        ++pPos;
    }
    if (!element.empty())
    {
        m_que.push(element);
        element.clear();
    }

    return 0;
}

int Calculator::calculation(void)
{
#if 0
    while (!m_que.empty())
    {
        cout << m_que.front() << " ";
        m_que.pop();
    }
    cout << endl;
#endif
    m_stk.push("0");
    int cnt = 0;
    while (!m_que.empty())
    {
        bool isnum = true;
        string element = m_que.front();
        m_que.pop();
        string elementNext = m_que.front();

        //cout << "cnt: [" << ++cnt << "] element: " << element  << endl;
        //sleep(1);
        //cout << "!isnum: " << element << endl;
        if (!this->isNum(element))
        {
            if (!this->isNum(elementNext)
                    && !this->isLeftBracket(elementNext) && !this->isRightBracket(elementNext)
                    && !this->isLeftBracket(element) && !this->isRightBracket(element))
            {
                continue;
            }
            isnum = false;
            //cout << "isnum is false...." << endl;
        }
        //cout << "element: " << element << endl;

        if (!isnum)
        {
            if (!isRightBracket(element))
            {
                //cout << "oprt: [" << element << "]" << endl;
                m_stk.push(element);
            }
            else
            {
                /* 右括号 */
                m_isBack = true;
                element = m_stk.top();
                //cout << "right bracket ..., element: [" << element << "]" << endl;
#if 0
                while (m_stk.size() > 0)
                {
                    cout << "val: [" << m_stk.top() << "]" << endl;
                    m_stk.pop();
                }
                exit(-1);
#endif
                m_stk.pop();
            }
        }
        if (isnum || m_isBack)
        {
            int ret = 0;
            do {
                ret = this->processNum(element);
            } while (1 == ret);
            if (0 == ret)
            {
                continue;
            }
        }
    }

    m_results = to_string(m_calRes);

    return 0;
}

int Calculator::processNum(string &element)
{
    string elementStk = m_stk.top();
    string elementNext = m_que.front();
    //cout << "eleSTK: " << elementStk << "   next: [" << elementNext << "]" << endl;
    if (isOprt(elementStk))
    {
        if (isOprt(elementNext))
        {
            if (!this->oprtCompare(elementStk, elementNext))
            {
                //cout << "stk: [" << elementStk << "], next: [" << elementNext << "]" << endl; 
                m_stk.push(element);
                return 0;
            }
        }

        m_stk.pop();
        string base = m_stk.top();
        m_stk.pop();
        m_calRes = stod(base);
        m_calRes = calculationVal(m_calRes, stod(element), *elementStk.c_str());
        element = to_string(m_calRes);
        if (m_stk.size() > 0)
        {
            return 1;
        }
        m_stk.push(to_string(m_calRes));
        cout << "cal: " << m_calRes << endl;
    }
    else if (isLeftBracket(elementStk))
    {
        //cout << "i am a left bracket" << endl;
        //cout << "elementStk: " << elementStk << endl;
        if (m_isBack)
        {
            m_stk.pop();
            m_isBack = false;
            return 1;
        }
        else
        {
            m_stk.push(element);
            return 0;
        }
    }
    else if (m_stk.size() == 1)
    {
        m_stk.pop();
        m_stk.push(element);
    }
    else
    {
        cout << "--------error--------, element: [" << element << "], stk: [" << elementStk << "]" << endl;
    }

    return 0;
}

void Calculator::clear()
{
    cout << "clear ..." << endl;
    m_parttern = PATTERN_E::NORMAL;

    m_rawValue.clear();
    std::stack<std::string> tmpstk;
    std::queue<std::string> tmpque;
    m_stk.swap(tmpstk);
    m_que.swap(tmpque);

    m_results.clear();
    m_calRes = 0;

    m_element.clear();

    m_errno = ERRNO_E::SUCCESS;

    m_isBack = false;

    return;
}

void Calculator::calPrint() const
{
    switch (m_errno)
    {
        case ERRNO_E::SUCCESS:cout << "res: " << m_results << endl;break;
        case ERRNO_E::INPUT_ERR:cout << "inpout error!" << endl;break;
        default: cout << "unknown error!" << endl;break;
    }

    return;
}

void Calculator::readFormual(string formualFile)
{
    ifstream ifs(formualFile, std::ios::in);
    string line;
    string::size_type pos = 0;
    while (getline(ifs, line))
    {
        pos = line.find_first_of('=') + 1;
        cout << line << endl;
        m_formual.push_back(line.substr(pos, line.length() - pos));
    }

#if 1
    cout << endl;
    //cout << "formual.size: " << m_formual.size() << endl;
    size_t idx;
    for (idx = 0; idx < m_formual.size(); ++idx)
    {
        cout << "======================" << endl;
        cout << m_formual[idx] << endl;
        this->handle(m_formual[idx]);
        this->clear();
    }
#endif
}


int main(int argc, char **argv)
{
    Calculator cal;
    cal.readFormual("./formual.txt");

    return 0;
}

