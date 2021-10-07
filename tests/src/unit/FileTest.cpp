#include "ks_file.h"

TEST_CASE("Reading and Writing into a File in the File System", "[Read and Write Test]"){
    kronos::ComponentFileManager* fileManager = new kronos::ComponentFileManager("File Manager", "C:");
    kronos::BusSync* openFileBus = new kronos::BusSync(KS_EVENT_CODE_OPEN_FILE, "openFile");
    openFileBus->AddReceivingComponent(fileManager);
    fileManager->Init();

    kronos::FileOpenMessage message;
    message.path = "/test.txt";
    message.mode = KS_OPEN_MODE_WRITE_ONLY | KS_OPEN_MODE_CREATE;
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    CHECK_FALSE(file == nullptr);
    const char str[10] = "Read test";
    char buffer[100];
    CHECK(file->Read(buffer, sizeof (buffer)) == KS_SUCCESS);
    CHECK(strncmp(buffer, "Read test", 10) == 0);
    CHECK(file->Write(str, sizeof (str)) == KS_SUCCESS);
    file->Close();
}

TEST_CASE("Renaming a File in the File System", "[Rename Test]"){
    kronos::ComponentFileManager* fileManager = new kronos::ComponentFileManager("File Manager", "C:");
    kronos::BusSync* openFileBus = new kronos::BusSync(KS_EVENT_CODE_OPEN_FILE, "openFile");
    openFileBus->AddReceivingComponent(fileManager);
    fileManager->Init();

    kronos::FileOpenMessage message;
    message.path = "/test.txt";
    message.mode = KS_OPEN_MODE_WRITE_READ; //change readonly
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    CHECK_FALSE(file == nullptr);
    file->Rename("newtest", "/");
    CHECK(file.path == "/newtest.txt");
    file->Close();
}

TEST_CASE("Moving in Directory then Rename File Test", "[Moving in Directory then Rename Test]"){
    kronos::ComponentFileManager* fileManager = new kronos::ComponentFileManager("File Manager", "C:");
    kronos::BusSync* openFileBus = new kronos::BusSync(KS_EVENT_CODE_OPEN_FILE, "openFile");
    openFileBus->AddReceivingComponent(fileManager);
    fileManager->Init();

    kronos::FileOpenMessage message;
    message.path = "/test.txt";
    message.mode = KS_OPEN_MODE_WRITE_READ; //change readonly
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    CHECK_FALSE(file, nullptr);
    file->Rename("newtest.txt", "/newdir/");
    CHECK(file.path == "/newdir/newtest.txt")
    file->Close();
}

TEST_CASE("File Stress Test", "[Stress Test]"){
    kronos::ComponentFileManager* fileManager = new kronos::ComponentFileManager("File Manager", "C:");
    kronos::BusSync* openFileBus = new kronos::BusSync(KS_EVENT_CODE_OPEN_FILE, "openFile");
    openFileBus->AddReceivingComponent(fileManager);
    fileManager->Init();

    kronos::FileOpenMessage message;
    message.path = "/test.txt";
    message.mode = KS_OPEN_MODE_WRITE_READ; //change readonly
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    CHECK_FALSE(file, nullptr);
    file->Close();
}

TEST_CASE("File Removal Test", "[File Removal Test]"){
    kronos::ComponentFileManager* fileManager = new kronos::ComponentFileManager("File Manager", "C:");
    kronos::BusSync* openFileBus = new kronos::BusSync(KS_EVENT_CODE_OPEN_FILE, "openFile");
    openFileBus->AddReceivingComponent(fileManager);
    fileManager->Init();

    kronos::FileOpenMessage message;
    message.path = "/newdir/newtest.txt";
    message.mode = KS_OPEN_MODE_WRITE_READ; //change readonly
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    CHECK_FALSE(file == nullptr);
    file = nullptr;
    CHECK(file == nullptr);
    file->Close();
}
