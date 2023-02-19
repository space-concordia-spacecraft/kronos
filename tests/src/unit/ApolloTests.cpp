#include "unit/ApolloTests.h"

using namespace kronos;

KT_TEST(ExportTest) {
    // Open file
    File file;
    file.Open("/apollo_test.txt");
    KT_ASSERT(&file, "UNABLE TO OPEN FILE");

    // Create exporter
    ApolloExporter apolloExporter(&file, {
            { "Test 1", KS_APOLLO_INT },
            { "Test 2", KS_APOLLO_INT }
    });

    // Write data
    KT_ASSERT(apolloExporter.WriteRow({ 69, 420 }) == ks_success, "UNABLE TO WRITE DATA TO FILE");

    return true;
}

KT_TEST(ImportTest) {
    // Open file
    File file;
    file.Open("/apollo_test.txt");
    KT_ASSERT(&file, "UNABLE TO OPEN FILE");

    // Create importer
    ApolloImporter apolloImporter(&file);
    auto headers = apolloImporter.GetHeaders();
    KT_ASSERT(headers.size() == 2, "HEADER SIZE DOESN'T MATCH");
    KT_ASSERT(headers[0].name == "Test 1", "HEADER DATA MISMATCH");
    KT_ASSERT(headers[0].dataType == KS_APOLLO_INT, "HEADER DATA MISMATCH");
    KT_ASSERT(headers[1].name == "Test 2", "HEADER DATA MISMATCH");
    KT_ASSERT(headers[1].dataType == KS_APOLLO_INT, "HEADER NAMES DON'T MATCH");

    // Read data
    std::vector<uint32_t> data;
    apolloImporter.ReadRow(data);
    KT_ASSERT(data[0] == 69, "DATA DOESN'T MATCH");
    KT_ASSERT(data[1] == 420, "DATA DOESN'T MATCH");

    apolloImporter.Close();

    return true;
}



