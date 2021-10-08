#pragma once

#include "kronos.h"
#include "Assert.h"

#define KT_TEST(func) bool func()

// Defines the array of test groups to execute
#define KT_TESTS(...) \
    namespace ktest {\
        int s_numTestGroups = KT_NARG(__VA_ARGS__) - 1;\
        TestGroup s_testGroups[] = {\
            __VA_ARGS__\
        };\
    }

// Defines one test group with a name and a set of unit tests
#define KT_TEST_GROUP(name, ...) \
    TestGroup(#name, (UnitTest[]){\
        __VA_ARGS__\
    }, KT_NARG(__VA_ARGS__) - 1),

// Defines a single unit test inside a test group
#define KT_UNIT_TEST(name, desc, func) UnitTest({ #name, desc, &func }),


namespace ktest {

    struct UnitTest {
        const char* name = nullptr;
        const char* description = nullptr;
        bool (*testFunction)() = nullptr;
    };

    class TestGroup {

    public:
        TestGroup(const char* name, const UnitTest* tests, const int numTests);

        void RunUnitTests();
        inline const char* GetName() { return m_name; }

    private:
        const char* m_name;
        const int m_numUnitTests;
        const UnitTest* m_unitTests;

    };

    extern TestGroup s_testGroups[];
    extern int s_numTestGroups;

    void RunTests();

}


