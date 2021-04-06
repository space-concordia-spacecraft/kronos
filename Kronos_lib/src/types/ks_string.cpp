#include "ks_string.h"

Ks_String::Ks_String(const char * str) : deallocate(false), str(str) {
    int i = 0;
    while (str[i] != '\0')
        i++;
    length = i;
}

Ks_String::Ks_String(const char * str, bool deallocate) : deallocate(deallocate), str(str) {
    int i = 0;
    while (str[i] != '\0')
        i++;
    length = i;
}

Ks_String::~Ks_String() {
    if (deallocate) {
        delete[] str;
    }
}