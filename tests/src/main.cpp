#include "KronosTest.h"

#include "unit/VectorTests.h"
#include "unit/QueueTests.h"
#include "unit/StringTests.h"
#include "unit/HashMapTests.h"

int main() {
    kronos::Framework::Init();
    ktest::RunTests();
}

KT_TESTS(
    KT_TEST_GROUP(VectorTests,
        KT_UNIT_TEST(VectorAddTest, "Description of the test.")
        KT_UNIT_TEST(VectorContainsTest, "Description of the test.")
        KT_UNIT_TEST(VectorRemoveTest, "Description of the test.")
        KT_UNIT_TEST(VectorClearTest, "Description of the test.")
        KT_UNIT_TEST(VectorCapacityTest, "Description of the test.")
    )

    KT_TEST_GROUP(QueueTests,
        KT_UNIT_TEST(QueuePushAndPopTest,"Description of the test.")
        KT_UNIT_TEST(QueuePeekTest,"Description of the test.")
        KT_UNIT_TEST(QueueClearTest,"Description of the test.")
        KT_UNIT_TEST(QueueCapacityTest,"Description of the test.")
    )

    KT_TEST_GROUP(StringTests,
        /* Functions */
        KT_UNIT_TEST(StringSizeTest, "Verifies that the kronos::String::Size() works as expected.")
        KT_UNIT_TEST(StringConcatenateTest, "Verifies the kronos::String::Concatenate() works as expected." )
        KT_UNIT_TEST(StringSubstringTest, "Verifies the kronos::String::Substring() works as expected." )

        /* Operators */
        KT_UNIT_TEST(StringComparisonTest, "Verifies that the kronos::String::operator!= works as expected.")
        KT_UNIT_TEST(StringAssignmentTest, "Verifies that the kronos::String::operator= works as expected.")
        KT_UNIT_TEST(StringAdditionTest, "Verifies that the kronos::String::operator+ works as expected.")
        KT_UNIT_TEST(StringAdditionAssignmentTest, "Verifies that the kronos::String::operator+= works as expected.")
        KT_UNIT_TEST(StringIndexTest, "Verifies that the kronos::String::operator[] works as expected.")
    )

    KT_TEST_GROUP(HashMapTests,
        /* Functions */
        KT_UNIT_TEST(MapPut, "Verifies that the kronos::HashMap::Put() works as expected.")
        KT_UNIT_TEST(MapRemove, "Verifies that the kronos::HashMap::Remove() works as expected.")
    )
)