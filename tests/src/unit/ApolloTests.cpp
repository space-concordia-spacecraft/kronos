#include "unit/ApolloTests.h"
#include "ks_apollo_format.h"
#include "ks_file_manager.h"

using namespace kronos;

extern ComponentFileManager* fileManager;

KT_TEST(ExportTest){
    File* file = fileManager->Open("/apollo_test.txt",KS_OPEN_MODE_CREATE | KS_OPEN_MODE_WRITE_ONLY);

    KT_ASSERT(file,"UNABLE TO OPEN FILE");

    Vector<ApolloHeader> headers;
    headers.Add({"Test", KS_APOLLO_INT});

    ApolloExporter apolloExporter(file,headers);

    Vector<uint32_t> data;
    data.Add(69420);

    apolloExporter.WriteRow(data);
    file->Close();
    return true;
}

KT_TEST(ImportTest){
    File* file = fileManager->Open("/apollo_test.txt", KS_OPEN_MODE_READ_ONLY);
    ApolloImporter apolloImporter(file);

    KT_ASSERT(apolloImporter.GetHeaders().Size() == 1, "HEADER SIZE DOESN'T MATCH");

    Vector<uint32_t> data;
    apolloImporter.ReadRow(data);
    printf("%lu\n", data[0]);

    file->Close();

    KT_ASSERT(data[0] == 69420, "DATA DOESN'T MATCH");

    return true;
}



