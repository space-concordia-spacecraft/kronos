#pragma once


class String {

private:
    const bool deallocate;
    const char * str;
    int length;

    String(const char * str, bool deallocate);

public:
    String(const char * str);
    ~String();

    inline const char * ptr() const {
        return str;
    }

    inline int size() const {
        return length;
    }

    static inline String concatenate(const String& first, const String& second) {
        char * newStr = new char[first.length + second.length + 1];

        // TODO replace with memory copying functions
        for (int i = 0; i < first.length; i++)
            newStr[i] = first.str[i];
        for (int i = 0; i <= second.length; i++)
            newStr[i + first.length] = second.str[i];

        newStr[first.length + second.length - 1] = '\0';

        return String(newStr, true);
    }

    friend String operator+(const String& left, const String& right) {
        return concatenate(left, right);
    }

    String operator+=(const String& right) {
        String a = *this;
        return concatenate(a, right);
    }

    String operator=(const String& target) {
        return target;
    }

};