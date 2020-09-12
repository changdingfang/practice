//***************************************************************
// @file:    String.h
// @author:  dingfang
// @date    2020-09-02 19:35:54
//***************************************************************

#ifndef __STRING_H__
#define __STRING_H__

#include <iostream>

namespace df
{


    class String final
    {
    public:
        typedef unsigned int sizeType;
        static constexpr sizeType npos = -1;
        static constexpr sizeType baseCapacity = 15;
        // TODO
        /* 15字节以内的使用栈 */

        static int compare(const char *data1, const char *data2, String::sizeType size);

    public:
        String();
        explicit String(const char *initValue);
        explicit String(const String &r);
        String(String &&r);
        ~String();

        String &operator = (const String &r);
        // TODO
        /* 需要优化字符串赋值操作, 减少delete和new次数 */
        String &operator = (const char *data);
        String &operator = (String &&r);
        
        // assign();

        /* * * * * * * * * * * * */
        /* 元素访问 */
        // TODO
        /* 优化需要识别读写操作,减少读时的内存分配 */
        char &operator [] (int index);
        const char &operator [] (int index) const;
        char &front();
        char &back();
        inline const char *data() const noexcept { return pValue_->data(); }
        inline const char *c_str() const noexcept { return pValue_->data(); }
        char &at(String::sizeType pos);
        /* * * * * * * * * * * * */


        /* * * * * * * * * * * * */
        /* 容量 */
        inline bool empty() const noexcept { return pValue_->size() == 0; }
        inline sizeType size() const noexcept { return pValue_->size(); }
        inline sizeType length() const noexcept { return this->size(); }
        inline sizeType max_size() const noexcept { return pValue_->max_size(); }
        inline sizeType capacity() const noexcept { return pValue_->capacity(); }
        void reserve(sizeType newCap = 0);
        void shrink_to_fit() noexcept;
        /* * * * * * * * * * * * */


        /* * * * * * * * * * * * */
        /* 操作 */
        void clear();
        // TODO
        // void insert();
        String &erase(sizeType index = 0, sizeType count = String::npos);
        void push_back(const char &c);
        void pop_back();
        String &append(const String &r);
        String &append(const String &r, sizeType pos, sizeType count);
        String &append(const char *data);
        String &append(const char *data, sizeType count); /* append和+=都依赖于这个 */
        String &append(sizeType count, char ch);
        String &operator += (const String &r);
        String &operator += (const char *data);
        String &operator += (char ch);
        int compare(const String &r) const;
        int compare(sizeType pos, sizeType count, const String &r) const;
        int compare(sizeType pos1, sizeType count1, const String &r, sizeType pos2, sizeType count2) const;
        int compare(const char *data) const;
        int compare(sizeType pos, sizeType count, const char *data) const;
        int compare(sizeType pos1, sizeType count1, const char *data, sizeType pos2, sizeType count2) const;
        // TODO
        // replace()
        String substr(sizeType pos = 0, sizeType count = String::npos) const;
        sizeType copy(char  *data, sizeType count, sizeType pos = 0) const;
        void resize(sizeType count, char ch = '\0');
        void swap(String &r) noexcept;
        /* * * * * * * * * * * * */


        // TODO
        /* * * * * * * * * * * * */
        /* 查找 */
        // find()
        // rfine()
        // find_first_of()
        // find_first_not_of()
        // find_last_of()
        // find_last_not_of()
        /* * * * * * * * * * * * */

        sizeType refCount() const noexcept { return pValue_->refCount(); }

        // TODO
        // operator >> ();
        // getline();

    private:
        void refCountCheck_()
        {
            if (pValue_->refCount() > 1)
            {
                pValue_->minusRefCount();
                pValue_ = new StringValue(pValue_->data());
            }
        }

    private:
        class StringValue final
        {
        public:
            explicit StringValue(const char *initValue);
            ~StringValue();

            StringValue(const StringValue &) = delete;
            StringValue(StringValue &&) = delete;
            StringValue &operator = (const StringValue &) = delete;
            StringValue &operator = (StringValue &&) = delete;

            inline char *data() const noexcept { return data_; }
            inline sizeType size() const noexcept { return size_; }
            inline sizeType capacity() const noexcept { return capacity_; }
            inline sizeType max_size() const noexcept { return (String::npos - 1) / 2; }
            inline sizeType addRefCount() noexcept { return ++refCount_; }
            inline sizeType minusRefCount() noexcept { return --refCount_; }
            inline sizeType refCount() const noexcept { return refCount_; }
            inline bool shareable() const noexcept { return shareable_; }
            inline void markUnshareable() noexcept { shareable_ = false; }
            void reserve(sizeType newCap);
            void shrink_to_fit() noexcept;

            void clear();
            void erase(sizeType index, sizeType count);
            void append(const char *data, sizeType count);
            void resize(sizeType count, char ch);

        private:
            bool      shareable_;
            sizeType  refCount_;
            sizeType  size_;
            sizeType  capacity_;
            char      *data_;
        };

        StringValue *pValue_;
    };

    bool operator == (const String &l, const String &r);
    bool operator == (const String &l, const char *data);
    bool operator == (const char *data, const String &r);
    std::ostream &operator << (std::ostream &s, const String &r);


    inline int min(String::sizeType num1, String::sizeType num2);


}; /* end of namespace df */


#endif /* __STRING_H__ */


