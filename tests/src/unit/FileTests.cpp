#include "KronosTest.h"
#include "ks_file.h"

using namespace kronos;

ComponentFileManager* fileManager;

KT_TEST(FileInitTest) {
    // Create file manager
    fileManager = new ComponentFileManager("File Manager", "C:");

    // Init file manager
    KsResult initResult = fileManager->Init();
    KT_ASSERT(initResult == KS_SUCCESS, "UNABLE TO INITIALIZE FILE MANAGER");

    return true;
}

KT_TEST(FileReadWriteTest) {
    File* file = fileManager->Open("/test.txt", KS_OPEN_MODE_WRITE_ONLY | KS_OPEN_MODE_CREATE);
    KT_ASSERT(file);

    // Write to file
    const char str[10] = "Read test";
    char buffer[100];
    KT_ASSERT(file->Write(str, sizeof(str)) == KS_SUCCESS);
    delete file;

    // Re-open and read from file
    file = fileManager->Open("/test.txt", KS_OPEN_MODE_READ_ONLY);
    KT_ASSERT(file);
    KT_ASSERT(file->Read(buffer, sizeof(buffer)) == KS_SUCCESS);
    KT_ASSERT(strncmp(buffer, "Read test", 10) == 0);
    file->Close();

    return true;
}

KT_TEST(FileRenameTest) {
    File* file = fileManager->Open("/test.txt", KS_OPEN_MODE_WRITE_READ);

    KT_ASSERT(file);
    KT_ASSERT(file->Rename("newtest", "/"));
    delete file;

    return true;
}

KT_TEST(FileMoveRenameTest) {
    File* file = fileManager->Open("/test.txt", KS_OPEN_MODE_WRITE_READ);

    KT_ASSERT(file);
    KT_ASSERT(file->Rename("newtest.txt", "/newdir/")); // TODO - Look into implementation of Rename()
    delete file;

    return true;
}

KT_TEST(FileStressTest) {
    File* file = fileManager->Open("/test.txt", KS_OPEN_MODE_WRITE_READ);
    KT_ASSERT(file);
    delete file;

    return true;
}

KT_TEST(FileRemovalTest) {
    File* file = fileManager->Open("/test.txt", KS_OPEN_MODE_WRITE_READ);
    KT_ASSERT(file);
    // TODO - Call file.Remove();
    delete file;

    return true;
}
