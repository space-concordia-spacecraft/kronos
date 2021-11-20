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
    KT_TEST_GROUP(ApolloTests,KT_UNIT_TEST(ExportTest,"Attempts to write to a file using the ApolloFormat."))
    KT_TEST_GROUP(ApolloTests,KT_UNIT_TEST(ImportTest, "Attempts to read the file that was created by the export,."))
)
