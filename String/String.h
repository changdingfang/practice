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

    class String
    {
    public:
        typedef unsigned int sizeType;
        static const sizeType npos = -1;

    public:
        String();
        String(const char *initValue);
        String(const String &r);
        ~String();

        String &operator =(const String &r);

        char &operator [](int index);
        char &front();
        char &back();
        inline const char *data() const { return pValue_->data; }
        inline const char *c_str() const { return pValue_->data; }
        char &at(String::sizeType pos);



        inline bool empty() const { return pValue_->size == 0; }
        inline sizeType size() const { return pValue_ ? pValue_->size : 0; }
        sizeType refCount() const { return pValue_->refCount; }

        friend std::ostream &operator <<(std::ostream &s, const String &r);

    private:
        class StringValue
        {
        public:
            StringValue(const char *initValue);
            ~StringValue();
            bool         shareable_;
            sizeType refCount;
            sizeType size;
            char         *data;
        };

        StringValue *pValue_;
    };

    std::ostream &operator <<(std::ostream &s, const String &r);
}; /* end of namespace df */


#endif
