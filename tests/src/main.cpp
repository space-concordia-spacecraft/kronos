#include "KronosTest.h"

#include "unit/VectorTests.h"
#include "unit/QueueTests.h"
#include "unit/StringTests.h"
#include "unit/HashMapTests.h"
#include "unit/FileTests.h"
#include "unit/ApolloTests.h"
int main() {
    kronos::Framework::Init();
    ktest::RunTests();
}

KT_TESTS(
    KT_TEST_GROUP(ApolloTests,KT_UNIT_TEST(ExportTest,"Description of the test."))
)
