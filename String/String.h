// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     String.h
// Author:       dingfang
// CreateDate:   2020-09-02 19:35:54
// ModifyAuthor: dingfang
// ModifyDate:   2020-09-20 11:46:38
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#ifndef __STRING_H__
#define __STRING_H__

#include <iostream>

namespace df
{


    class String final
    {
    public:
        typedef unsigned long int sizeType;
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
        String &insert(sizeType index, sizeType count, char ch);
        String &insert(sizeType index, const char *data);
        String &insert(sizeType index, const char *data, sizeType count);
        String &insert(sizeType index, const String &r);
        String &insert(sizeType index, const String &r, sizeType indexR, sizeType count = String::npos);

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
        String &replace(sizeType pos, sizeType count, const String &r);
        String &replace(sizeType pos, sizeType count, const String &r, sizeType pos2, sizeType count2 = String::npos);
        String &replace(sizeType pos, sizeType count, const char *data);
        String &replace(sizeType pos, sizeType count, const char *data, sizeType count2);
        String &replace(sizeType pos, sizeType count, sizeType count2, char ch);
        String substr(sizeType pos = 0, sizeType count = String::npos) const;
        sizeType copy(char  *data, sizeType count, sizeType pos = 0) const;
        void resize(sizeType count, char ch = '\0');
        void swap(String &r) noexcept;
        /* * * * * * * * * * * * */


        // TODO
        /* * * * * * * * * * * * */
        /* 查找 */
        /* 采用的比较暴力,可优化成KMP算法 */
        sizeType find(const String &r, sizeType pos = 0) const;
        sizeType find(const char *data, sizeType pos = 0) const;
        sizeType find(const char *data, sizeType pos, sizeType count) const;
        sizeType find(const char ch, sizeType pos = 0) const;
        sizeType rfind(const String &r, sizeType pos = 0) const;
        sizeType rfind(const char *data, sizeType pos = 0) const;
        sizeType rfind(const char *data, sizeType pos, sizeType count) const;
        sizeType rfind(const char ch, sizeType pos = 0) const;
        sizeType find_first_of(const String &r, sizeType pos = 0) const;
        sizeType find_first_of(const char *data, sizeType pos = 0) const;
        sizeType find_first_of(const char *data, sizeType pos, sizeType count) const;
        sizeType find_first_of(const char ch, sizeType pos = 0) const;
        sizeType find_first_not_of(const String &r, sizeType pos = 0) const;
        sizeType find_first_not_of(const char *data, sizeType pos = 0) const;
        sizeType find_first_not_of(const char *data, sizeType pos, sizeType count) const;
        sizeType find_first_not_of(const char ch, sizeType pos = 0) const;
        sizeType find_last_of(const String &r, sizeType pos = String::npos) const;
        sizeType find_last_of(const char *data, sizeType pos = String::npos) const;
        sizeType find_last_of(const char *data, sizeType pos, sizeType count) const;
        sizeType find_last_of(const char ch, sizeType pos = String::npos) const;
        sizeType find_last_not_of(const String &r, sizeType pos = String::npos) const;
        sizeType find_last_not_of(const char *data, sizeType pos = String::npos) const;
        sizeType find_last_not_of(const char *data, sizeType pos, sizeType count) const;
        sizeType find_last_not_of(const char ch, sizeType pos = String::npos) const;
        /* * * * * * * * * * * * */

        sizeType refCount() const noexcept { return pValue_->refCount(); }

    private:
        /* 查找 */
        /* 
         * 采用的比较暴力,且把后项查找和前项查找基层到一起,效率更低, 
         * 可优化成KMP算法,后项查找可分离出来反向查 
         * */
        sizeType find_(const char *data, sizeType pos, sizeType count, sizeType num) const;

        inline void refCountCheck_()
        {
            if (pValue_->refCount() > 1)
            {
                pValue_->minusRefCount();
                pValue_ = new StringValue(pValue_->data());
            }
        }

        inline void refCountDeleteCheck_()
        {
            if (pValue_->minusRefCount() == 0)
            {
                delete pValue_;
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
            void replace(sizeType pos, sizeType count, const char *data, sizeType count2);
            void replace(sizeType pos, sizeType count, sizeType count2, char ch);

        private:
            void replacePre_(sizeType pos, sizeType count, sizeType count2);

        private:
            bool      shareable_;
            sizeType  refCount_;
            sizeType  size_;
            sizeType  capacity_;
            char      *data_;
        };

        StringValue *pValue_;
    };

    inline char eof() { return _GLIBCXX_STDIO_EOF; /* -1 */ };

    String operator + (const String &l, const String &r);
    String operator + (const String &l, const char *data);
    String operator + (const char *data, const String &r);
    String operator + (const String &l, char ch);
    String operator + (char ch, const String &r);
    bool operator == (const String &l, const String &r);
    bool operator == (const String &l, const char *data);
    bool operator == (const char *data, const String &r);
    std::ostream &operator << (std::ostream &s, const String &r);
    std::istream &operator >> (std::istream &s, String &r);
    template <class Char_T, class Is>
        Is &getline(Is &s, Char_T &r)
        {
            r.erase();
            String::sizeType count = 0;
            char ch = static_cast<char>(s.rdbuf()->sgetc());
            char buf[128];
            const char eof = df::eof();
            std::ios_base::iostate err = std::ios_base::goodbit;
            String::sizeType len = 0;

            while (count < r.max_size() && ch != eof && ch != '\n')
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
            r.append(buf, len);

            if (ch == eof)
            {
                err |= std::ios_base::eofbit;
            }
            else if (ch == '\n')
            {
                ++count;
                s.rdbuf()->sbumpc();
            }
            else
            {
                err |= std::ios_base::failbit;
            }

            if (!count)
            {
                err |= std::ios_base::failbit;
            }

            if (err)
            {
                s.setstate(err);
            }

            return s;
        }


    inline int min(String::sizeType num1, String::sizeType num2);


}; /* end of namespace df */


#endif /* __STRING_H__ */


