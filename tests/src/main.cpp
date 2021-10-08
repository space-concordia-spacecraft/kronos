#include "KronosTest.h"

#include "unit/VectorTests.h"

int main() {
    kronos::Framework::Init();
    ktest::RunTests();
}

KT_TESTS(
    KT_TEST_GROUP(VectorTests,
        KT_UNIT_TEST(VectorAddTest, "Description of the test.", VectorAddTest)
        KT_UNIT_TEST(VectorContainsTest, "Description of the test.", VectorContainsTest)
        KT_UNIT_TEST(VectorRemoveTest, "Description of the test.", VectorRemoveTest)
        KT_UNIT_TEST(VectorClearTest, "Description of the test.", VectorClearTest)
        KT_UNIT_TEST(VectorCapacityTest, "Description of the test.", VectorCapacityTest)
    )
)