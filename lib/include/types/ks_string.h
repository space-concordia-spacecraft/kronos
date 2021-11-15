#pragma once

#include <cstring>
#include "asf.h"

namespace kronos {

    class String {
    public:
        String(const char* str) {
            m_Length = 0;
            while (str[m_Length] != '\0')
                m_Length++;
            char* newStr = new char[m_Length + 1];
            memcpy(newStr, str, m_Length + 1);
            m_String = newStr;
        }

        String(const String& from) : m_Length(from.m_Length) {
            char* str = new char[m_Length + 1];
            memcpy(str, from.m_String, m_Length + 1);
            m_String = str;
        }

        ~String() {
            delete[] m_String;
        }

        const char* Ptr() const { return m_String; }

        size_t Size() const { return m_Length; }

        size_t Hash() const {
            size_t hash = 5381;
            for (size_t i = 0; i < m_Length; i++)
                hash = hash * 33 + m_String[i];
            return hash;
        }

        size_t Find(const char *str) const {
            char * ptr = strstr(m_String, str);
            if(ptr == nullptr)
                return UINT32_MAX;

            return ptr - m_String;
        }

        String Substring(size_t start) const {
            return Substring(start, m_Length);
        }

        String Substring(size_t start, size_t end) const {
            if (start >= m_Length || end > m_Length || end <= start)
                return "";

            return String(*this, start, end);
        }

        static String Concatenate(const String& first, const String& second) {
            char* newStr = new char[first.m_Length + second.m_Length + 1];

            memcpy(newStr, first.m_String, first.m_Length);
            memcpy(newStr + first.m_Length, second.m_String, second.m_Length);
            newStr[first.m_Length + second.m_Length] = '\0';

            String result(newStr);
            delete[] newStr;

            return result;
        }

        friend String operator+(const String& left, const String& right) {
            return String::Concatenate(left, right);
        }

        String& operator+=(const String& right) {
            *this = Concatenate(*this, right);
            return *this;
        }

        String& operator=(const String& other) {
            if (this == &other)
                return *this;

            // Delete previous buffer
            delete[] m_String;

            // Allocate new buffer
            char* newStr = new char[other.m_Length + 1];
            m_Length = other.m_Length;
            memcpy(newStr, other.m_String, other.m_Length + 1);
            m_String = newStr;

            return *this;
        }

        friend bool operator==(const String& lhs, const String& rhs) {
            if (lhs.m_Length != rhs.m_Length) {
                return false;
            }
            for (size_t i = 0; i < lhs.m_Length + 1; i++) {
                if (lhs.m_String[i] != rhs.m_String[i]) {
                    return false;
                }
            }
            return true;
        }

        friend bool operator!=(const String& lhs, const String& rhs) {
            return !(lhs == rhs);
        }

        char operator[](size_t idx) {
            return m_String[idx];
        }

    private:
        String(const String& from, size_t start, size_t end) {
            char* str = new char[end - start + 1];
            memcpy(str, from.m_String + start, end - start);
            str[end - start] = '\0';
            m_String = str;
            m_Length = end - start;
        }

        const char* m_String;
        size_t m_Length;
    };

}