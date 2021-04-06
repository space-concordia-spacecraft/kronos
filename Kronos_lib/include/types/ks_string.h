#pragma once


class Ks_String {

private:
    const bool deallocate;
    const char * str;
    int length;

    Ks_String(const char * str, bool deallocate);

public:
    Ks_String(const char * str);
    ~Ks_String();

    inline const char * ptr() const {
        return str;
    }

    inline int size() const {
        return length;
    }

    static inline Ks_String concatenate(const Ks_String& first, const Ks_String& second) {
        char * newStr = new char[first.length + second.length + 1];

        // TODO replace with memory copying functions
        for (int i = 0; i < first.length; i++)
            newStr[i] = first.str[i];
        for (int i = 0; i <= second.length; i++)
            newStr[i + first.length] = second.str[i];

        newStr[first.length + second.length - 1] = '\0';

        return Ks_String(newStr, true);
    }

    friend Ks_String operator+(const Ks_String& left, const Ks_String& right) {
        return concatenate(left, right);
    }

    Ks_String operator+=(const Ks_String& right) {
        Ks_String a = *this;
        return concatenate(a, right);
    }

    Ks_String operator=(const Ks_String& target) {
        return target;
    }

};