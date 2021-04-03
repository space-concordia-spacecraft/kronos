#include "types/string.h"

String::String(const char * str) : deallocate(false), str(str) {
    int i = 0;
    while (str[i] != '\0')
        i++;
    length = i;
}

String::String(const char * str, bool deallocate) : deallocate(deallocate), str(str) {
    int i = 0;
    while (str[i] != '\0')
        i++;
    length = i;
}

String::~String() {
    if (deallocate) {
        delete[] str;
    }
}