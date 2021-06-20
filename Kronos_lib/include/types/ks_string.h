#pragma once

#include <iostream>

namespace kronos {

    class String {

    private:
        bool deallocate;
        const char * str;
        size_t length;

        String(const char * str, bool deallocate) : deallocate(deallocate), str(str) {
            int i = 0;
            while (str[i] != '\0')
                i++;
            length = i;
        }

    public:
        //Constructor
        String(const char * str) : deallocate(false), str(str) {
            int i = 0;
            while (str[i] != '\0')
                i++;
            length = i;
        }

        //Destructor
        ~String() {
            if (deallocate) {
                delete[] str;
            }
        }

        inline const char * ptr() const {
            return str;
        }

        inline size_t size() const {
            return length;
        }

        //Concatenate function
        static inline String concatenate(const String & first, const String & second) {
            char * newStr = new char[first.length + second.length + 1];

            // TODO replace with memory copying functions
            for (uint32_t i = 0; i < first.length; i++)
                newStr[i] = first.str[i];
            for (uint32_t i = 0; i < second.length; i++)
                newStr[first.length + i] = second.str[i];

            newStr[first.length + second.length] = '\0';

            return String(newStr, true);
        }

        // + operator overload
        friend String operator+(const String & left, const String & right) {
            return concatenate(left, right);
        }
        // += operator overload
        String & operator+=(const String & right) {
            *this = concatenate(*this, right);

            return *this;
        }

        //= operator overload
        String & operator=(const String & other) {
            if (this == &other)
                return *this;

            // Delete previous buffer
            if (deallocate) {
                delete[] str;
            }

            // Allocate new buffer
            char * newStr = new char[other.length + 1];
            length = other.length;
            deallocate = true;

            // TODO replace with memory copying function
            for (uint32_t i = 0; i <= other.length; i++)
                newStr[i] = other.str[i];

            str = newStr;
            return *this;
        }

        //== operator overload
        friend bool operator==(const String& lhs, const String& rhs) {
            if (lhs.length != rhs.length) {
                return false;
            }
            for (uint32_t i = 0; i <= lhs.length; i++) {
                if (lhs.str[i] != rhs.str[i]) {
                    return false;
                }
            }


            return true;
        }

        //!= operator overload
        friend bool operator!=(const String& lhs, const String& rhs) {
            if (lhs.length != rhs.length) {
                return true;
            }
            for (uint32_t i = 0; i <= lhs.length; i++) {
                if (lhs.str[i] != rhs.str[i]) {
                    return true;
                }
            }


            return false;
        }

        //[index] operator overload
        char operator[](uint32_t idx) {


            return str[idx];
        }
    };

}