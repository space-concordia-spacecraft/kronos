#pragma once

#undef printf
#undef scanf

extern "C" {
namespace {
    int printf(const char* fmt, ...);
    int scanf(const char* fmt, ...);
}
}
