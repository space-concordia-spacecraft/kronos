#pragma once

#include "kronos.h"
#include "Assert.h"

#define KT_TEST(func) bool func()

#define KT_INIT namespace ktest {\
                TestGroup s_testGroups[] = {

#define KT_NUM_TEST_GROUPS(num) namespace ktest { int s_numTestGroups = num; }
#define KT_TEST_GROUP_START(name) TestGroup(#name, {
#define KT_TEST_GROUP_END }),

#define KT_UNIT_TEST(name, desc, func) { #name, desc, &func },

#define KT_END };}

#define KT_MAX_UNIT_TESTS_PER_GROUP 10

namespace ktest {

    struct UnitTest {
        const char* name = nullptr;
        const char* description = nullptr;
        bool (*testFunction)() = nullptr;
    };

    class TestGroup {

    public:
        TestGroup(const char* name, const UnitTest (&tests)[KT_MAX_UNIT_TESTS_PER_GROUP]);

        void RunUnitTests();

    private:
        const char* m_name;
        const UnitTest (&m_unitTests)[KT_MAX_UNIT_TESTS_PER_GROUP];

    };

    extern TestGroup s_testGroups[];
    extern int s_numTestGroups;

    void RunTests();

}

