#pragma once

#include "asf.h"

namespace kronos {

    class String {
    private:
        String(const char* str, bool deallocate);

    public:
        String(const char* str);
        String(const String& from);
        ~String();

        const char* Ptr() const;
        size_t Size() const;

        static String Concatenate(const String& first, const String& second);

        friend String operator+(const String& left, const String& right);
        String& operator+=(const String& right);
        String& operator=(const String& other);

        friend bool operator==(const String& lhs, const String& rhs);
        friend bool operator!=(const String& lhs, const String& rhs);

        char operator[](size_t idx);

    private:
        bool m_Deallocate;
        const char* m_String;
        size_t m_Length;
    };

}