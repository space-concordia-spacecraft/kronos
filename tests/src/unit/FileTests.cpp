#include "KronosTest.h"

using namespace kronos;

KT_TEST(FileInitTest) {
    // Init file manager
    return true;
}

KT_TEST(FileReadWriteTest) {
    File file;
    file.Open("/test.txt");

    KT_ASSERT(&file);

    // Write to file
    const char str[10] = "Read test";
    char buffer[100];
    KT_ASSERT(file.Write(str, sizeof(str)) == 10);

    file.Close();

    // Re-open and read from file
    file.Open("/test.txt");
    KT_ASSERT(&file);
    KT_ASSERT(file.Read(buffer, sizeof(buffer)) == 10);
    KT_ASSERT(strncmp(buffer, "Read test", 10) == 0);
    file.Close();

    return true;
}

KT_TEST(FileRemovalTest) {
    File file;
    auto result = file.Remove("/test.txt");
    KT_ASSERT(result == ks_success, "UNABLE TO REMOVE FILE.");

    return true;
}