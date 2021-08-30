#include "ks_file.h"

class FileTest : public ::testing::Test{

protected:
    kronos::ComponentFileManager* fileManager;
    kronos::BusSync* openFileBus;

    virtual void SetUp() {
        fileManager = new kronos::ComponentFileManager("File Manager", "C:");
        openFileBus = new kronos::BusSync(KS_EVENT_CODE_OPEN_FILE, "openFile");
        openFileBus->AddReceivingComponent(fileManager);
        fileManager->Init();
    }

    virtual void TearDown() {
    }
};

TEST_F(FileTest, WriteTest){
    kronos::FileOpenMessage message;
    message.path = "/test.txt";
    message.mode = KS_OPEN_MODE_WRITE_ONLY | KS_OPEN_MODE_CREATE;
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    EXPECT_NE(file, nullptr);
    const char str[10] = "Read test";
    EXPECT_EQ(file->Write(str, sizeof (str)), KS_SUCCESS);
    file->Close();
}

TEST_F(FileTest, ReadTest){
    kronos::FileOpenMessage message;
    message.path = "/test.txt";
    message.mode = KS_OPEN_MODE_READ_ONLY; //change readonly
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    EXPECT_NE(file, nullptr);
    char buffer[100];
    EXPECT_EQ(file->Read(buffer, sizeof (buffer)), KS_SUCCESS);
    EXPECT_EQ(strncmp(buffer, "Read test", 10), 0);
    file->Close();
}

TEST_F(FileTest, RenameTest){
    kronos::FileOpenMessage message;
    message.path = "/test.txt";
    message.mode = KS_OPEN_MODE_WRITE_READ; //change readonly
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    EXPECT_NE(file, nullptr);
    file->Rename("newtest", "/");
    EXPECT_EQ(file.path, "/newtest.txt")
}

TEST_F(FileTest, MoveDirectoryThenRenameTest){
    kronos::FileOpenMessage message;
    message.path = "/test.txt";
    message.mode = KS_OPEN_MODE_WRITE_READ; //change readonly
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    EXPECT_NE(file, nullptr);
    file->Rename("newtest.txt", "/newdir/");
    EXPECT_EQ(file.path, "/newdir/newtest.txt")
}

TEST_F(FileTest, StressTest){
    kronos::FileOpenMessage message;
    message.path = "/test.txt";
    message.mode = KS_OPEN_MODE_WRITE_READ; //change readonly
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    EXPECT_NE(file, nullptr);
}

TEST_F(FileTest, RemoveTest){
    kronos::FileOpenMessage message;
    message.path = "/newdir/newtest.txt";
    message.mode = KS_OPEN_MODE_WRITE_READ; //change readonly
    kronos::File* file = openFileBus->PublishSync<kronos::FileOpenMessage, kronos::File>(&message);
    EXPECT_NE(file, nullptr);
    file = nullptr;
    EXPECT_EQ(file, nullptr);
}
