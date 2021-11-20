#include "unit/ApolloTests.h"
#include "ks_apollo_format.h"
#include "ks_file_manager.h"

using namespace kronos;
KT_TEST(ExportTest){
    ComponentFileManager *fileManager = new ComponentFileManager("NULL","C:");
    fileManager->Init();
    File* file = fileManager->Open("/test.txt",KS_OPEN_MODE_CREATE | KS_OPEN_MODE_WRITE_ONLY);
    KT_ASSERT(file,"UNABLE TO OPEN FILE");
    Vector<ApolloHeader> headers;
    headers.Add({"Test",KS_APOLLO_INT});
    ApolloExporter* apolloExporter = new ApolloExporter(file,headers);
    Vector<uint32_t> data;
    data.Add(69420);
    apolloExporter->WriteRow(data);
    KT_ASSERT(false,"APOLLO TEST"); //PRINTS IF FALSE/CONDITION FAILS
    return true;
}

KT_TEST(ImportTest){
    char buffer[100];
    ComponentFileManager *fileManager = new ComponentFileManager("NULL","C:");
    File* file = fileManager->Open("test.txt",KS_OPEN_MODE_READ_ONLY);
    file->Read(buffer,sizeof(buffer));
    printf(buffer);
    return true;
}



