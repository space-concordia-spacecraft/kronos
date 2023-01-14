#include "KronosTest.h"

#include "unit/QueueTests.h"
#include "unit/FileTests.h"
#include "unit/ApolloTests.h"

int main() {
    kronos::Framework::Init();
    ktest::RunTests();
}

KT_TESTS(
    KT_TEST_GROUP(
        QueueTests,
        KT_UNIT_TEST(QueuePushAndPopTest, "Description of the test.")
    KT_UNIT_TEST(QueuePeekTest, "Description of the test.")
    KT_UNIT_TEST(QueueClearTest, "Description of the test.")
    KT_UNIT_TEST(QueueCapacityTest, "Description of the test.")
)

    KT_TEST_GROUP(FileTests,
    KT_UNIT_TEST(FileInitTest, "Verifies that the kronos::File Properly Initializes.")
    KT_UNIT_TEST(FileReadWriteTest, "Verifies that the kronos::File Properly Reads and Writes into a File in the File System.")
    KT_UNIT_TEST(FileRemovalTest, "Verifies that the kronos File Removal Test works as expected.")
)

    KT_TEST_GROUP(ApolloTests,
    KT_UNIT_TEST(ExportTest, "Attempts to write to a file using the ApolloFormat.")
    KT_UNIT_TEST(ImportTest, "Attempts to read the file that was created by the export.")
)

//    KT_TEST_GROUP(TelemetryLoggerTests,
//       KT_UNIT_TEST(TelemetryLoggerWriteTest,"Attempts to write to a file using the tlm log.")
//       KT_UNIT_TEST(TelemetryLoggerReadTest, "Attempts to read the file that was created by the tlm log.")
//    )
)
