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
        // TODO
        // String(String &&r);
        ~String();

        String &operator = (const String &r);
        // TODO
        /* 需要优化字符串赋值操作, 减少delete和new次数 */
        String &operator = (const char *data);
        // TODO
        // String &operator = (String &&r);
        
        // assign();

        /* * * * * * * * * * * * */
        /* 元素访问 */
        // TODO
        /* 优化需要识别读写操作,减少读时的内存分配 */
        char &operator [] (int index);
        char &front();
        char &back();
        inline const char *data() const { return pValue_->data(); }
        inline const char *c_str() const { return pValue_->data(); }
        char &at(String::sizeType pos);
        /* * * * * * * * * * * * */


        /* * * * * * * * * * * * */
        /* 容量 */
        inline bool empty() const { return pValue_->size() == 0; }
        inline sizeType size() const { return pValue_->size(); }
        inline sizeType length() const { return this->size(); }
        inline sizeType max_size() const { return pValue_->max_size(); }
        inline sizeType capacity() const { return pValue_->capacity(); }
        void reserve(sizeType newCap = 0);
        void shrink_to_fit() noexcept;
        /* * * * * * * * * * * * */


        // TODO
        /* * * * * * * * * * * * */
        /* 操作 */
        void clear();
        // void insert();
        // erase()
        // push_back()
        // pop_back()
        String &append(const String &r);
        String &append(const char *data); /* append和+=都依赖于这个 */
        String &append(const char &ch);
        String &operator += (const String &r);
        String &operator += (const char *data);
        String &operator += (const char &ch);
        int compare(const String &r) const;
        int compare(sizeType pos, sizeType count, const String &r) const;
        int compare(sizeType pos1, sizeType count1, const String &r, sizeType pos2, sizeType count2) const;
        // TODO
        /* 改进成返回第几个字符不同 */
        int compare(const char *data) const;
        // replace()
        // substr()
        // copy()
        // resize()
        // swap()
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

        sizeType refCount() const { return pValue_->refCount(); }

        friend bool operator == (const String &l, const String &r);
        friend std::ostream &operator << (std::ostream &s, const String &r);
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
        class StringValue
        {
        public:
            StringValue(const char *initValue);
            ~StringValue();

            inline char *data() const { return data_; }
            inline sizeType size() const { return size_; }
            inline sizeType capacity() const { return capacity_; }
            inline sizeType max_size() const { return (String::npos - 1) / 2; }
            inline sizeType addRefCount() { return ++refCount_; }
            inline sizeType minusRefCount() { return --refCount_; }
            inline sizeType refCount() const { return refCount_; }
            inline bool shareable() const { return shareable_; }
            inline void markUnshareable() { shareable_ = false; }
            void reserve(sizeType newCap);
            void shrink_to_fit() noexcept;

            void clear();
            void append(const char *data);

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
    std::ostream &operator << (std::ostream &s, const String &r);


    int min(String::sizeType num1, String::sizeType num2);


}; /* end of namespace df */


#endif /* __STRING_H__ */


