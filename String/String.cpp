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
        : refCount_(1)
          , capacity_(baseCapacity)
          , shareable_(true)
    {
        size_ = ::strlen(initValue);
        if (size_ >= String::npos)
        {
            throw "string too long";
        }

        while (size_ / capacity_)
        {
            if (size_ == capacity_)
            {
                break;
            }
            capacity_ <<= 1;
            if (capacity_ > this->max_size())
            {
                capacity_ = this->max_size();
                break;
            }
        }

        data_ = new char [capacity_ + 1];
        ::memset(data_, 0x00, capacity_ + 1);
        ::strcpy(data_, initValue);
    }


    String::StringValue::~StringValue()
    {
        capacity_ = 0;
        size_ = 0;
        delete [] data_;
        data_ = nullptr;
    }


    void String::StringValue::reserve(sizeType newCap)
    {
        /*
         * 1. newCap > capacity() && newCap < 2 * capacity()
         *   扩充到: 2 * capacity()
         * 2. newCap > 2 * capacity()
         *   扩充到: newCap
         * 3. newCap <= capacity() && newCap >= baseCapacity
         *   缩减到: newCap
         * 4. newCap <= baseCapacity
         *   缩减到: baseCapacity
         * */
        if (newCap < this->size())
        {
            newCap = this->size();
        }

        if (newCap < String::baseCapacity)
        {
            newCap = String::baseCapacity;
        }

        if (newCap > this->capacity() && newCap < 2 * this->capacity())
        {
            newCap = 2 * this->capacity();
            if (newCap > this->max_size())
            {
                newCap = this->max_size();
            }
        }

        char *data = new char [newCap + 1];
        ::strcpy(data, data_);
        delete [] data_;
        data_ = data;
        capacity_ = newCap;
    }


    void String::StringValue::shrink_to_fit() noexcept
    {
        if (this->capacity() > this->size())
        {
            try
            {
                this->reserve(0);
            }
            catch(...) { }
        }
    }


    void String::StringValue::clear()
    {
        size_ = 0;
        data_[0] = '\0';
    }


    void String::StringValue::append(const char *data)
    {
        sizeType size = ::strlen(data) + this->size();
        if (size > this->max_size())
        {
            throw("string too long");
        }

        if (size > this->capacity())
        {
            this->reserve(size);
        }
        ::strcat(data_, data);
        size_ = size;
        data_[this->size()] = '\0';
    }


    String::String()
        : pValue_(new String::StringValue(""))
    { }


    String::String(const char *initValue)
        : pValue_(new String::StringValue(initValue))
    { }


    String::String(const String &r)
    {
        if (r.pValue_->shareable())
        {
            pValue_ = r.pValue_;
            pValue_->addRefCount();
        }
        else
        {
            pValue_ = new StringValue(r.pValue_->data());
        }
    }


    String::~String()
    {
        if (pValue_->minusRefCount() == 0)
        {
            delete pValue_;
            pValue_ = nullptr;
        }
    }


    String &String::operator = (const String &r)
    {
        if (pValue_ == r.pValue_)
        {
            return *this;
        }

        if (pValue_->minusRefCount() == 0)
        {
            delete pValue_;
        }

        if (r.pValue_->shareable())
        {
            pValue_ = r.pValue_;
            pValue_->addRefCount();
            return *this;
        }

        pValue_ = new StringValue(r.pValue_->data());

        return *this;
    }


    String &String::operator = (const char *data)
    {
        if (pValue_->minusRefCount() == 0)
        {
            delete pValue_;
        }

        pValue_ = new StringValue(data);

        return *this;
    }


    char &String::operator [] (int index)
    {
        /* 悲观认为只要是该操作符就认为会修改,不再共享 */
        this->refCountCheck_();
        pValue_->markUnshareable();

        return pValue_->data()[index];
    }


    char &String::front()
    {
        this->refCountCheck_();
        pValue_->markUnshareable();

        return pValue_->data()[0];
    }


    char &String::back()
    {
        this->refCountCheck_();
        pValue_->markUnshareable();

        return pValue_->data()[pValue_->size() - 1];
    }


    char &String::at(String::sizeType pos)
    {
        if (pos >= pValue_->size())
        {
            throw "pos >= size";
        }

        this->refCountCheck_();
        pValue_->markUnshareable();

        return pValue_->data()[pos];
    }


    void String::reserve(sizeType newCap)
    {
        pValue_->reserve(newCap);
    }


    void String::shrink_to_fit() noexcept
    {
        pValue_->shrink_to_fit();
    }


    void String::clear()
    {
        if (pValue_->refCount() > 1)
        {
            pValue_->minusRefCount();
            pValue_ = new StringValue("");
            return ;
        }
        pValue_->clear();
    }


    String &String::append(const String &r)
    {
        return this->append(r.data());
    }


    String &String::append(const char *data)
    {
        this->refCountCheck_();
        pValue_->append(data);
        return *this;
    }


    String &String::append(const char &ch)
    {
        const char tmp[2] = { ch };
        return this->append(tmp);
    }


    String &String::operator += (const String &r)
    {
        return this->append(r.data());
    }


    String &String::operator += (const char *data)
    {
        return this->append(data);
    }


    String &String::operator += (const char &ch)
    {
        return this->append(ch);
    }


    int String::compare(const String &r) const
    {
        return this->compare(r.data());
    }


    int String::compare(sizeType pos, sizeType count, const String &r) const
    {
        if (pos > this->size())
        {
            throw("pos > size");
        }

        if (pos + count > this->size())
        {
            count = this->size() - pos;
        }

        const sizeType size = min(count, r.size());
        int res = compare(this->data() + pos, r.data(), size);

        return res ? res : !(this->size() == r.size());
    }


    int String::compare(sizeType pos1, sizeType count1, const String &r, sizeType pos2, sizeType count2) const
    {
        if (pos1 > this->size() || pos2 > r.size())
        {
            throw("pos > size");
        }

        if (pos1 + count1 > this->size())
        {
            count1 = this->size() - pos1;
        }

        if (pos2 + count2 > r.size())
        {
            count2 = r.size() - pos2;
        }

        const sizeType size = min(count1, count2);
        int res = compare(this->data() + pos1, r.data() + pos2, size);

        return res ? res : !(this->size() == r.size());
    }


    int String::compare(const char *data) const
    {
        const sizeType size = min(this->size(), sizeType(::strlen(data)));
        int res = compare(this->data(), data, size);

        return res ? res : !(this->size() == ::strlen(data));
    }


    bool operator == (const String &l, const String &r)
    {
        return !::strcmp(l.data(), r.data());
    }


    std::ostream &operator << (std::ostream &s, const String &r)
    {
        return r.size() == 0 ? s : s << r.pValue_->data();
    }


    int String::compare(const char *data1, const char *data2, String::sizeType size)
    {
        return ::strncmp(data1, data2, size);
    }


    int min(String::sizeType num1, String::sizeType num2)
    {
        return num1 > num2 ? num2 : num1;
    }



}; /* end of namespace df */
