#include "KronosTest.h"

namespace ktest {

    TestGroup::TestGroup(const char* name, const UnitTest (&tests)[KT_MAX_UNIT_TESTS_PER_GROUP])
        : m_name(name), m_unitTests(tests) {}

    void TestGroup::RunUnitTests() {
        for (int i = 0; i < KT_MAX_UNIT_TESTS_PER_GROUP; i++) {
            if (m_unitTests[i].testFunction == nullptr)
                continue;
            printf("Running test '%s' ...\n", m_unitTests[i].name);
            bool result = m_unitTests[i].testFunction();
            printf("Test %s\n", result ? "SUCCEEDED" : "FAILED");
            printf("\n");
        }
    }

    void RunTests() {
        for (int i = 0; i < s_numTestGroups; i++) {
            s_testGroups[i].RunUnitTests();
        }
    }

}