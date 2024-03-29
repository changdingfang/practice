// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     String.cpp
// Author:       dingfang
// CreateDate:   2020-09-02 19:40:15
// ModifyAuthor: dingfang
//  ModifyDate:   2021-10-28 22:51:41
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "String.h"
#include <string.h>

using namespace std;

namespace df
{


    String::StringValue::StringValue(const char *initValue)
        : refCount_(1)
          , isLocal_(false)
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

        if (capacity_ <= String::baseCapacity)
        {
            isLocal_ = true;
            data_ = localBuf_;
        }
        else
        {
            data_ = new char [capacity_ + 1];
        }
        ::memset(data_, 0x00, capacity_ + 1);
        ::strcpy(data_, initValue);
    }


    String::StringValue::~StringValue()
    {
        if (!isLocal_)
        {
            delete [] data_;
            data_ = nullptr;
        }
        capacity_ = 0;
        size_ = 0;
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

        if (newCap <= String::baseCapacity)
        {
            if (!isLocal_)
            {
                ::memcpy(localBuf_, data_, this->size());
                delete [] data_;
                data_ = localBuf_;
            }
            isLocal_ = true;
        }
        else
        {
            char *data = new char [newCap + 1];
            ::memcpy(data, data_, this->size());
            if (!isLocal_)
            {
                delete [] data_;
            }
            data_ = data;
            capacity_ = newCap;
            isLocal_ = false;
        }
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
        this->replacePre_(pos, count, count2);

        for (sizeType i = pos, j = 0; j < count2; ++i, ++j)
        {
            data_[i] = data[j];
        }
    }


    void String::StringValue::replace(sizeType pos, sizeType count, sizeType count2, char ch)
    {
        this->replacePre_(pos, count, count2);
        ::memset(data_ + pos, ch, count2);
    }


    void String::StringValue::replacePre_(sizeType pos, sizeType count, sizeType count2)
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
        if (pValue_->refCount() == 1)
        {
            pValue_->replace(0, this->size(), data, ::strlen(data));
        }
        else
        {
            StringValue *tmp = new StringValue(data);
            pValue_->minusRefCount();
            pValue_ = tmp;
        }

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


    String operator + (const String &l, const String &r)
    {
        return String(l.data()) + r.data();
    }


    String operator + (const char *data, const String &r)
    {
        return String(r.data()) + data;
    }


    String operator + (const String &l, const char *data)
    {
        return String(l.data()).append(data, ::strlen(data));
    }


    String operator + (const String &l, char ch)
    {
        return String(l.data()).append(1, ch);
    }


    String operator + (char ch, const String &r)
    {
        return String(r.data()) + ch;
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


    const char *String::data() const
    { 
        if (pValue_->refCount() > 1)
        {
            pValue_->minusRefCount();
            StringValue **p = const_cast<StringValue **>(&pValue_);
            *p = new StringValue(pValue_->data());
        }
        pValue_->markUnshareable();
        return pValue_->data();
    }


    const char *String::c_str() const
    {
        return this->data();
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
        int res = df::compare(this->data() + pos1, data + pos2, size);

        return res ? res : !(count1 == count2);
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
        pValue_->replace(pos, count, count2, ch);
        return *this;
    }


    String String::substr(sizeType pos, sizeType count) const
    {
        return String("").append(*this, pos, count);
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


    String::sizeType String::find(const String &r, sizeType pos) const
    {
        return this->find(r.data(), pos, r.size());
    }


    String::sizeType String::find(const char *data, sizeType pos) const
    {
        return this->find(data, pos, ::strlen(data));
    }


    String::sizeType String::find(const char *data, sizeType pos, sizeType count) const
    {
        return this->find_(data, pos, count, 1);
    }


    String::sizeType String::find(const char ch, sizeType pos) const
    {
        char tmp[ ] = {ch, '\0'};
        return this->find(tmp, pos, 1);
    }


    String::sizeType String::rfind(const String &r, sizeType pos) const
    {
        return this->rfind(r.data(), pos, r.size());
    }


    String::sizeType String::rfind(const char *data, sizeType pos) const
    {
        return this->rfind(data, pos, ::strlen(data));
    }


    String::sizeType String::rfind(const char *data, sizeType pos, sizeType count) const
    {
        return this->find_(data, pos, count, String::npos);
    }


    String::sizeType String::rfind(const char ch, sizeType pos) const
    {
        char tmp[ ] = {ch, '\0'};
        return this->rfind(tmp, pos, 1);
    }


    String::sizeType String::find_(const char *data, sizeType pos, sizeType count, sizeType num) const
    {
        if (count > this->size())
        {
            return String::npos;
        }

        sizeType lastPos = String::npos;
        sizeType currNum = 0;
        for (sizeType i = pos; i + count < this->size(); ++i)
        {
            sizeType j = 0, k = i;
            for (; j < count; ++j, ++k)
            {
                if (this->data()[k] != data[j])
                {
                    break;
                }
            }

            if (j == count)
            {
                lastPos = i;
                if (++currNum == num)
                {
                    break;
                }
            }
        }

        return lastPos;
    }


    String::sizeType String::find_first_of(const String &r, sizeType pos) const
    {
        return this->find_first_of(r.data(), pos, r.size());
    }


    String::sizeType String::find_first_of(const char *data, sizeType pos) const
    {
        return this->find_first_of(data, pos, ::strlen(data));
    }


    String::sizeType String::find_first_of(const char *data, sizeType pos, sizeType count) const
    {
        for (sizeType i = pos; i < this->size(); ++i)
        {
            for (sizeType j = 0; j < count; ++j)
            {
                if (this->data()[i] == data[j])
                {
                    return i;
                }
            }
        }
        return String::npos;
    }


    String::sizeType String::find_first_of(const char ch, sizeType pos) const
    {
        for (sizeType i = pos; i < this->size(); ++i)
        {
            if (this->data()[i] == ch)
            {
                return i;
            }
        }
        return String::npos;
    }


    String::sizeType String::find_first_not_of(const String &r, sizeType pos) const
    {
        return this->find_first_not_of(r.data(), pos, r.size());
    }


    String::sizeType String::find_first_not_of(const char *data, sizeType pos) const
    {
        return this->find_first_not_of(data, pos, ::strlen(data));
    }


    String::sizeType String::find_first_not_of(const char *data, sizeType pos, sizeType count) const
    {
        for (sizeType i = pos; i < this->size(); ++i)
        {
            sizeType j = 0;
            for (; j < count; ++j)
            {
                if (this->data()[i] == data[j])
                {
                    break;
                }
            }

            if (j == count)
            {
                return i;
            }
        }
        return String::npos;
    }


    String::sizeType String::find_first_not_of(const char ch, sizeType pos) const
    {
        for (sizeType i = pos; i < this->size(); ++i)
        {
            if (this->data()[i] != ch)
            {
                return i;
            }
        }

        return String::npos;
    }


    String::sizeType String::find_last_of(const String &r, sizeType pos) const
    {
        return this->find_last_of(r.data(), pos, r.size());
    }


    String::sizeType String::find_last_of(const char *data, sizeType pos) const
    {
        return this->find_last_of(data, pos, ::strlen(data));
    }


    String::sizeType String::find_last_of(const char *data, sizeType pos, sizeType count) const
    {
        if (pos >= this->size())
        {
            pos = this->size() - 1;
        }

        for (sizeType i = pos; i >= 0 && i != String::npos; --i)
        {
            for (sizeType j = 0; j < count; ++j)
            {
                if (this->data()[i] == data[j])
                {
                    return i;
                }
            }
        }

        return String::npos;
    }


    String::sizeType String::find_last_of(const char ch, sizeType pos) const
    {
        if (pos >= this->size())
        {
            pos = this->size() - 1;
        }

        for (sizeType i = pos; i >= 0 && i != String::npos; --i)
        {
            if (this->data()[i] == ch)
            {
                return i;
            }
        }

        return String::npos;
    }


    String::sizeType String::find_last_not_of(const String &r, sizeType pos) const
    {
        return this->find_last_not_of(r.data(), pos, r.size());
    }


    String::sizeType String::find_last_not_of(const char *data, sizeType pos) const
    {
        return this->find_last_not_of(data, pos, ::strlen(data));
    }


    String::sizeType String::find_last_not_of(const char *data, sizeType pos, sizeType count) const
    {
        if (pos >= this->size())
        {
            pos = this->size() - 1;
        }

        for (sizeType i = pos; i >= 0 && i != String::npos; --i)
        {
            sizeType j = 0;
            for (; j < count; ++j)
            {
                if (this->data()[i] == data[j])
                {
                    break;
                }
            }

            if (j == count)
            {
                return i;
            }
        }

        return String::npos;
    }


    String::sizeType String::find_last_not_of(const char ch, sizeType pos) const
    {
        if (pos >= this->size())
        {
            pos = this->size() - 1;
        }

        for (sizeType i = pos; i >= 0 && i != String::npos; --i)
        {
            if (this->data()[i] != ch)
            {
                return i;
            }
        }

        return String::npos;
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


    std::istream &operator >> (std::istream &s, String &r)
    {
        r.erase();
        String::sizeType count = 0;
        char ch = static_cast<char>(s.rdbuf()->sgetc());
        char buf[128];
        const char eof = df::eof();
        cout << "width: " << s.width() << endl;
        std::ios_base::iostate err = std::ios_base::goodbit;
        String::sizeType len = 0;

        while (count < r.max_size() && ch != eof && ch != '\n' && ch != ' ' && ch != '\t')
        {
            if (len == sizeof(buf))
            {
                r.append(buf, sizeof(buf));
                len = 0;
            }
            buf[len++] = ch;
            ch = static_cast<char>(s.rdbuf()->snextc());
            ++count;
        }

        if (ch == eof)
        {
            err |= std::ios_base::eofbit;
            s.width(0);
        }

        if (!count)
        {
            err |= std::ios_base::failbit;
        }

        if (err)
        {
            s.setstate(err);
        }

        r.append(buf, len);

        return s;
    }


    int compare(const char *data1, const char *data2, String::sizeType size)
    {
        return ::strncmp(data1, data2, size);
    }


    inline int min(String::sizeType num1, String::sizeType num2)
    {
        return num1 > num2 ? num2 : num1;
    }


}; /* end of namespace df */
