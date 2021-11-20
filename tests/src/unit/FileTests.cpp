#include "KronosTest.h"
#include "ks_file.h"

using namespace kronos;

ComponentFileManager* fileManager;

KT_TEST(FileInitTest){
    fileManager = new ComponentFileManager("File Manager", "C:");
    KsResult initResult = fileManager->Init();

    KT_ASSERT(initResult == KS_SUCCESS, "UNABLE TO INITIALIZE FILE MANAGER");

    return true;
}

KT_TEST(FileReadWriteTest){
    File* file = fileManager->Open("/test.txt",KS_OPEN_MODE_WRITE_ONLY | KS_OPEN_MODE_CREATE);
    KT_ASSERT(file);
    const char str[10] = "Read test";
    char buffer[100];
    KT_ASSERT(file->Write(str, sizeof (str)) == KS_SUCCESS);
    file->Close();
    delete file;
    file = nullptr;

    file = fileManager->Open("/test.txt", KS_OPEN_MODE_READ_ONLY);
    KT_ASSERT(file);

    KT_ASSERT(file->Read(buffer, sizeof (buffer)) == KS_SUCCESS);
    KT_ASSERT(strncmp(buffer, "Read test", 10) == 0);

    file->Close();

    return true;
}

KT_TEST(FileRenameTest){
    File* file = fileManager->Open("/test.txt", KS_OPEN_MODE_WRITE_READ);

    KT_ASSERT(file);
    KT_ASSERT(file->Rename("newtest", "/"));

    file->Close();

    return true;
}

KT_TEST(FileMoveRenameTest){
    File* file = fileManager->Open("/test.txt", KS_OPEN_MODE_WRITE_READ);

    KT_ASSERT(file);
    KT_ASSERT(file->Rename("newtest.txt", "/newdir/")); // TODO - Look into implementation of Rename()

    file->Close();

    return true;
}

KT_TEST(FileStressTest){
    File* file = fileManager->Open("/test.txt", KS_OPEN_MODE_WRITE_READ);
    KT_ASSERT(file);

    file->Close();

    return true;
}

KT_TEST(FileRemovalTest){
    File* file = fileManager->Open("/test.txt", KS_OPEN_MODE_WRITE_READ);
    KT_ASSERT(file);
    // TODO - Call file.Remove();
    file = nullptr;
    KT_ASSERT(file == nullptr);

    return true;
}
