#include "KronosTest.h"
#include "ks_file.h"

kronos::ComponentFileManager* fileManager;
kronos::BusSync* openFileBus;

KT_TEST(FileInitTest){
    fileManager = new kronos::ComponentFileManager("File Manager", "C:");
    openFileBus = new kronos::BusSync(KS_EVENT_CODE_OPEN_FILE, "openFile");
    openFileBus->AddReceivingComponent(fileManager);
    fileManager->Init();

    return true;
}

KT_TEST(FileReadWriteTest){
    kronos::FileOpenMessage message;
    message.path = "/test.txt";
    message.mode = KS_OPEN_MODE_WRITE_ONLY | KS_OPEN_MODE_CREATE;
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    KT_ASSERT(file);
    const char str[10] = "Read test";
    char buffer[100];
    KT_ASSERT(file->Write(str, sizeof (str)) == KS_SUCCESS);
    file->Close();
    delete file;
    file = nullptr;

    message.path = "/test.txt";
    message.mode = KS_OPEN_MODE_READ_ONLY;
    file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    KT_ASSERT(file);

    KT_ASSERT(file->Read(buffer, sizeof (buffer)) == KS_SUCCESS);
    KT_ASSERT(strncmp(buffer, "Read test", 10) == 0);

    file->Close();

    return true;
}

KT_TEST(FileRenameTest){
    kronos::FileOpenMessage message;
    message.path = "/test.txt";
    message.mode = KS_OPEN_MODE_WRITE_READ; //change readonly
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    KT_ASSERT(file);
    KT_ASSERT(file->Rename("newtest", "/"));
    file->Close();

    return true;
}

KT_TEST(FileMoveRenameTest){
    kronos::FileOpenMessage message;
    message.path = "/test.txt";
    message.mode = KS_OPEN_MODE_WRITE_READ; //change readonly
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    KT_ASSERT(file);
    KT_ASSERT(file->Rename("newtest.txt", "/newdir/")); // TODO - Look into implementation of Rename()
    file->Close();

    return true;
}

KT_TEST(FileStressTest){
    kronos::FileOpenMessage message;
    message.path = "/test.txt";
    message.mode = KS_OPEN_MODE_WRITE_READ; //change readonly
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    KT_ASSERT(file);
    file->Close();

    return true;
}

KT_TEST(FileRemovalTest){
    kronos::FileOpenMessage message;
    message.path = "/test.txt";
    message.mode = KS_OPEN_MODE_WRITE_READ; //change readonly
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    KT_ASSERT(file);
    // TODO - Call file.Remove();
    file = nullptr;
    KT_ASSERT(file == nullptr);


    return true;
}
