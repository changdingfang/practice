// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     String.cpp
// Author:       dingfang
// CreateDate:   2020-09-02 19:40:15
// ModifyAuthor: dingfang
// ModifyDate:   2020-09-13 14:59:09
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

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
            throw("string too long");
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
        if (newCap > this->max_size())
        {
            throw "length too long";
        }

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
        ::memcpy(data, data_, this->size());
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


    void String::StringValue::erase(String::sizeType index, String::sizeType count)
    {
        if (index > this->size())
        {
            throw("out of range");
        }

        if (index + count > this->size() || count > this->size())
        {
            count = this->size() - index;
        }

        sizeType i = index;
        for (; i + count < this->size(); ++i)
        {
            data_[i] = data_[i + count];
        }
        data_[i] = 0;
        size_ = i;
    }


    void String::StringValue::append(const char *data, sizeType count)
    {
        sizeType size = count + this->size();
        if (size > this->max_size())
        {
            throw("string too long");
        }

        if (size > this->capacity())
        {
            this->reserve(size);
        }

        ::memcpy(data_ + this->size(), data, count);
        size_ = size;
        data_[this->size()] = '\0';
    }


    void String::StringValue::resize(sizeType count, char ch)
    {
        if (count > this->max_size())
        {
            throw("string too long");
        }

        if (this->size() < count)
        {
            if (this->capacity() < count)
            {
                this->reserve(count);
            }
            ::memset(data_ + this->size(), ch, count - this->size());
        }
        size_ = count;
        data_[this->size()] = '\0';
    }


    void String::StringValue::replace(sizeType pos, sizeType count, const char *data, sizeType count2)
    {
        const sizeType oldSize = this->size();
        sizeType newSize = 0;
        sizeType pc = pos + count;

        if (pos > oldSize)
        {
            throw("pos > size()");
        }

        if (pc > oldSize)
        {
            count = oldSize - pos;
            newSize = pos + count2;
        }
        else
        {
            /* count2 > count  --> newSize = pos + count2 + (size_ - count - pos) */
            /* count2 <= count --> newSize = size_ - (count2 - count) */
            newSize = count2 + oldSize - count;
        }

        if (newSize > this->capacity())
        {
            this->reserve(newSize);
        }

        if (count2 <= count && pc < oldSize)
        {
            for (sizeType i = pos + count2, j = pc; j < oldSize; ++i, ++j)
            {
                data_[j] = data_[i];
            }
        }
        else if (count2 > count && pc < oldSize)
        {
            for (int i = oldSize, j = newSize; i >= pc && i != -1; --i, --j)
            {
                data_[j] = data_[i];
            }
        }

        for (sizeType i = pos, j = 0; j < count2; ++i, ++j)
        {
            data_[i] = data[j];
        }

        size_ = newSize;
        data_[size_] = '\0';
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


    String::String(String &&r)
    {
        pValue_ = r.pValue_;
        r.pValue_ = nullptr;
    }


    String::~String()
    {
        if (pValue_ && pValue_->minusRefCount() == 0)
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

        if (r.pValue_->shareable())
        {
            this->refCountDeleteCheck_();
            pValue_ = r.pValue_;
            pValue_->addRefCount();
        }
        else
        {
            StringValue *tmp = new StringValue(r.pValue_->data());
            this->refCountDeleteCheck_();
            pValue_ = tmp;
        }

        return *this;
    }


    String &String::operator = (const char *data)
    {
        StringValue *tmp = new StringValue(data);
        this->refCountDeleteCheck_();
        pValue_ = tmp;

        return *this;
    }


    String &String::operator = (String &&r)
    {
        if (pValue_ != r.pValue_)
        {
            this->refCountDeleteCheck_();
            pValue_ = r.pValue_;
            r.pValue_ = nullptr;
        }

        return *this;
    }


    char &String::operator [] (int index)
    {
        /* 悲观认为只要是该操作符就认为会修改,不再共享 */
        this->refCountCheck_();
        pValue_->markUnshareable();

        return pValue_->data()[index];
    }


    const char &String::operator [] (int index) const
    {
        /* 悲观认为只要是该操作符就认为会修改,不再共享 */
        if (pValue_->refCount() > 1)
        {
            pValue_->minusRefCount();
            StringValue **p = const_cast<StringValue **>(&pValue_);
            *p = new StringValue(pValue_->data());
        }
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
            throw("pos >= size");
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


    String &String::insert(sizeType index, sizeType count, char ch)
    {
        return this->replace(index, 0, count, ch);
    }


    String &String::insert(sizeType index, const char *data)
    {
        return this->replace(index, 0, data, ::strlen(data));
    }


    String &String::insert(sizeType index, const char *data, sizeType count)
    {
        return this->replace(index, 0, data, count);
    }


    String &String::insert(sizeType index, const String &r)
    {
        return this->replace(index, 0, r.data(), r.size());
    }


    String &String::insert(sizeType index, const String &r, sizeType indexR, sizeType count)
    {
        if (indexR > r.size())
        {
            throw("index > size()");
        }
        if (count + indexR > r.size())
        {
            count = r.size() - indexR;
        }

        return this->replace(index, 0, r.data() + indexR, count);
    }


    String &String::erase(sizeType index, sizeType count)
    {
        this->refCountCheck_();
        pValue_->erase(index, count);

        return *this;
    }


    void String::push_back(const char &c)
    {
        this->append(1, c);
    }


    void String::pop_back()
    {
        if (this->size() == 0)
        {
            return ;
        }
        this->erase(this->size() - 1, 1);
    }


    String &String::append(const String &r)
    {
        return this->append(r.data(), r.size());
    }


    String &String::append(const String &r, sizeType pos, sizeType count)
    {
        if (pos > r.size())
        {
            throw("out of range");
        }

        if (pos + count > r.size() || count == String::npos)
        {
            count = r.size() - pos;
        }

        return this->append(r.data() + pos, count);
    }


    String &String::append(const char *data)
    {
        return this->append(data, ::strlen(data));
    }


    String &String::append(const char *data, sizeType count)
    {
        this->refCountCheck_();
        pValue_->append(data, count);
        return *this;
    }


    String &String::append(sizeType count, char ch)
    {
        this->resize(this->size() + count, ch);
        return *this;
    }


    String &String::operator += (const String &r)
    {
        return this->append(r.data(), r.size());
    }


    String &String::operator += (const char *data)
    {
        return this->append(data, ::strlen(data));
    }


    String &String::operator += (char ch)
    {
        return this->append(1, ch);
    }


    int String::compare(const String &r) const
    {
        return this->compare(0, this->size(), r.data(), 0, r.size());
    }


    int String::compare(sizeType pos, sizeType count, const String &r) const
    {
        return this->compare(pos, count, r.data(), 0, r.size());
    }


    int String::compare(sizeType pos1, sizeType count1, const String &r, sizeType pos2, sizeType count2) const
    {
        return this->compare(pos1, count1, r.data(), pos2, count2);
    }


    int String::compare(const char *data) const
    {
        return this->compare(0, this->size(), data, 0, ::strlen(data));
    }


    int String::compare(sizeType pos, sizeType count, const char *data) const
    {
        return this->compare(pos, count, data, 0, ::strlen(data));
    }


    int String::compare(sizeType pos1, sizeType count1, const char *data, sizeType pos2, sizeType count2) const
    {
        sizeType dataSize = ::strlen(data);
        if (pos1 > this->size() || pos2 > dataSize)
        {
            throw("pos > size");
        }

        if (pos1 + count1 > this->size())
        {
            count1 = this->size() - pos1;
        }

        if (pos2 + count2 > dataSize)
        {
            count2 = dataSize - pos2;
        }

        const sizeType size = min(count1, count2);
        int res = compare(this->data() + pos1, data + pos2, size);

        return res ? res : !(count1 == count2);
    }


    void String::swap(String &r) noexcept
    {
        if (pValue_ == r.pValue_)
        {
            return ;
        }
        StringValue *p = pValue_;
        pValue_ = r.pValue_;
        r.pValue_ = p;
    }


    String &String::replace(sizeType pos, sizeType count, const String &r)
    {
        return this->replace(pos, count, r, 0, r.size());
    }


    String &String::replace(sizeType pos, sizeType count, const String &r, sizeType pos2, sizeType count2)
    {
        if (pos2 < r.size())
        {
            if (count2 == String::npos || count2 + pos2 > r.size())
            {
                count2 = r.size() - pos2;
            }

            this->replace(pos, count, r.data() + pos2, count2);
        }
        return *this;
    }


    String &String::replace(sizeType pos, sizeType count, const char *data)
    {
        return this->replace(pos, count, data, ::strlen(data));
    }


    String &String::replace(sizeType pos, sizeType count, const char *data, sizeType count2)
    {
        this->refCountCheck_();
        pValue_->replace(pos, count, data, count2);
        return *this;
    }


    String &String::replace(sizeType pos, sizeType count, sizeType count2, char ch)
    {
        if (count2 > this->max_size() || count2 == String::npos)
        {
            throw("count too long");
        }

        char *p = new char[count2];
        ::memset(p, ch, count2);

        try
        {
            this->replace(pos, count, p, count2);
        }
        catch(...) 
        { 
            delete [] p;
            throw; 
        }

        delete [] p;

        return *this;
    }


    String String::substr(sizeType pos, sizeType count) const
    {
        return std::move(String("").append(*this, pos, count));
    }


    String::sizeType String::copy(char  *data, sizeType count, sizeType pos) const
    {
        if (pos > this->size())
        {
            throw("out of range");
        }
        if (pos + count > this->size() || count == String::npos)
        {
            count = this->size() - pos;
        }
        ::memcpy(data, this->data() + pos, count);
        return count;
    }


    void String::resize(sizeType count, char ch)
    {
        pValue_->resize(count, ch);
    }


    bool operator == (const String &l, const String &r)
    {
        return !l.compare(r);
    }


    bool operator == (const String &l, const char *data)
    {
        return !l.compare(data);
    }


    bool operator == (const char *data, const String &r)
    {
        return !r.compare(data);
    }


    std::ostream &operator << (std::ostream &s, const String &r)
    {
        return r.size() == 0 ? s : s << r.data();
    }


    int String::compare(const char *data1, const char *data2, String::sizeType size)
    {
        return ::strncmp(data1, data2, size);
    }


    inline int min(String::sizeType num1, String::sizeType num2)
    {
        return num1 > num2 ? num2 : num1;
    }



}; /* end of namespace df */
