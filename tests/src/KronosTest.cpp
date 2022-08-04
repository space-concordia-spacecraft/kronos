#include "KronosTest.h"

namespace ktest {

    TestGroup::TestGroup(const char* name, const UnitTest* tests, const int numTests)
            : m_name(name), m_numUnitTests(numTests), m_unitTests(tests) {}

    void TestGroup::RunUnitTests() {
        for (int i = 0; i < m_numUnitTests; i++) {
            printf("Running test '%s' ...\n", m_unitTests[i].name);
            bool result = m_unitTests[i].testFunction();
            printf("Test %s\n", result ? "SUCCEEDED" : "FAILED");
            printf("\n");
        }
    }

    void RunTests(void *data) {
        for (int i = 0; i < s_numTestGroups; i++) {
            printf("======== %s ========\n\n", s_testGroups[i].GetName());
            s_testGroups[i].RunUnitTests();
        }
    }

}