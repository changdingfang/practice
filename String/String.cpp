 //***************************************************************
 // @file:    String.cpp
 // @author:  dingfang
 // @date    2020-09-02 19:40:15
 //***************************************************************
 
#include "String.h"
#include <string.h>

using namespace std;

namespace df
{
    String::StringValue::StringValue(const char *initValue)
        : refCount(1)
        , shareable_(true)
    {
        size = ::strlen(initValue);
        data = new char [size + 1];
        ::strcpy(data, initValue);
    }
    String::StringValue::~StringValue()
    {
        size = 0;
        delete [] data;
        data = NULL;
    }

    String::String()
        : pValue_(new String::StringValue(""))
    {
    }

    String::String(const char *initValue)
        : pValue_(new String::StringValue(initValue))
    {
    }

    String::String(const String &r)
    {
        if (r.pValue_->shareable_)
        {
            pValue_ = r.pValue_;
            ++pValue_->refCount;
        }
        else
        {
            pValue_ = new StringValue(r.pValue_->data);
        }
    }

    String::~String()
    {
        if (--pValue_->refCount == 0)
        {
            delete pValue_;
            pValue_ = NULL;
        }
    }

    String &String::operator =(const String &r)
    {
        if (pValue_ == r.pValue_)
        {
            return *this;
        }

        if (--pValue_->refCount == 0)
        {
            delete pValue_;
        }

        if (pValue_->shareable_)
        {
            pValue_ = r.pValue_;
            ++pValue_->refCount;
            return *this;
        }

        pValue_ = new StringValue(r.pValue_->data);

        return *this;
    }

    char &String::operator [](int index)
    {
        /* 悲观人为只要是该操作符就认为会修改,不再共享 */
        if (pValue_->refCount > 1)
        {
            --pValue_->refCount;
            pValue_ = new StringValue(pValue_->data);
        }

        pValue_->shareable_ = false;

        return pValue_->data[index];
    }

    char &String::front()
    {
        if (pValue_->size == 0)
        {
            /* 返回一个越界字符 */
            return pValue_->data[1];
        }
        if (pValue_->refCount > 1)
        {
            --pValue_->refCount;
            pValue_ = new StringValue(pValue_->data);
        }

        pValue_->shareable_ = false;

        return pValue_->data[0];
    }

    char &String::back()
    {
        if (pValue_->size == 0)
        {
            /* 返回一个越界字符 */
            return pValue_->data[1];
        }
        if (pValue_->refCount > 1)
        {
            --pValue_->refCount;
            pValue_ = new StringValue(pValue_->data);
        }

        pValue_->shareable_ = false;

        return pValue_->data[pValue_->size - 1];
    }

    char &String::at(String::sizeType pos)
    {
        if (pos >= pValue_->size)
        {
            throw "pos >= size";
        }

        if (pValue_->refCount > 1)
        {
            --pValue_->refCount;
            pValue_ = new StringValue(pValue_->data);
        }

        pValue_->shareable_ = false;

        return pValue_->data[pos];
    }

    std::ostream &operator <<(std::ostream &s, const String &r)
    {
        return s << r.pValue_->data;
    }

}; /* end of namespace df */
