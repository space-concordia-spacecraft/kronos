#include "KronosTest.h"

using namespace kronos;

uint32_t GetValue1Tick() {
    static uint32_t val;
    return val++;
}

uint32_t GetValue2Tick() {
    static uint32_t val;
    return val++;
}

KT_TEST(TelemetryLoggerWriteTest) {
    std::vector<TelemetryChannel> channels1Tick;
    channels1Tick.push_back({"temperature", &GetValue1Tick });
    channels1Tick.push_back({"batteryLevel", &GetValue1Tick });
    channels1Tick.push_back({"radiation", &GetValue1Tick });
    channels1Tick.push_back({"attitude", &GetValue1Tick });

    std::vector<TelemetryChannel> channels2Tick;
    channels2Tick.push_back({"temperature", &GetValue2Tick });
    channels2Tick.push_back({"batteryLevel", &GetValue2Tick });
    channels2Tick.push_back({"radiation", &GetValue2Tick });
    channels2Tick.push_back({"attitude", &GetValue2Tick });

    ComponentTelemetryLogger logger("telemetryLogger");
    logger.AddTelemetryGroup("telemetryLoggerTest1Tick", 1, channels1Tick);
    logger.AddTelemetryGroup("telemetryLoggerTest2Tick", 2, channels2Tick);

    EventMessage msg;
    msg.eventCode = KS_EVENT_CODE_RATE_GROUP_TICK;

    for (int i = 0; i < 5; i++) {
        logger.ProcessEvent(msg);
    }

    return true;
}

KT_TEST(TelemetryLoggerReadTest) {
    // Open file
    File* file1Tick = ComponentFileManager::Get().Open("/telemetryLoggerTest1Tick.txt", KS_OPEN_MODE_READ_ONLY);
    KT_ASSERT(file1Tick, "UNABLE TO OPEN FILE");

    File* file2Tick = ComponentFileManager::Get().Open("/telemetryLoggerTest2Tick.txt", KS_OPEN_MODE_READ_ONLY);
    KT_ASSERT(file2Tick, "UNABLE TO OPEN FILE");

    // Create importer
    ApolloImporter apolloImporter1Tick(file1Tick);
    auto headers1Tick = apolloImporter1Tick.GetHeaders();
    KT_ASSERT(headers1Tick.size() == 4, "HEADER SIZE DOESN'T MATCH");
    KT_ASSERT(headers1Tick[0].name == "temperature", "HEADER DATA MISMATCH");
    KT_ASSERT(headers1Tick[0].dataType == KS_APOLLO_FLOAT, "HEADER DATA MISMATCH");
    KT_ASSERT(headers1Tick[1].name == "batteryLevel", "HEADER DATA MISMATCH");
    KT_ASSERT(headers1Tick[1].dataType == KS_APOLLO_FLOAT, "HEADER NAMES DON'T MATCH");
    KT_ASSERT(headers1Tick[2].name == "radiation", "HEADER DATA MISMATCH");
    KT_ASSERT(headers1Tick[2].dataType == KS_APOLLO_FLOAT, "HEADER NAMES DON'T MATCH");
    KT_ASSERT(headers1Tick[3].name == "attitude", "HEADER DATA MISMATCH");
    KT_ASSERT(headers1Tick[3].dataType == KS_APOLLO_FLOAT, "HEADER NAMES DON'T MATCH");

    // Create importer
    ApolloImporter apolloImporter2Tick(file2Tick);
    auto headers2Tick = apolloImporter2Tick.GetHeaders();
    KT_ASSERT(headers2Tick.size() == 4, "HEADER SIZE DOESN'T MATCH");
    KT_ASSERT(headers2Tick[0].name == "temperature", "HEADER DATA MISMATCH");
    KT_ASSERT(headers2Tick[0].dataType == KS_APOLLO_FLOAT, "HEADER DATA MISMATCH");
    KT_ASSERT(headers2Tick[1].name == "batteryLevel", "HEADER DATA MISMATCH");
    KT_ASSERT(headers2Tick[1].dataType == KS_APOLLO_FLOAT, "HEADER NAMES DON'T MATCH");
    KT_ASSERT(headers2Tick[2].name == "radiation", "HEADER DATA MISMATCH");
    KT_ASSERT(headers2Tick[2].dataType == KS_APOLLO_FLOAT, "HEADER NAMES DON'T MATCH");
    KT_ASSERT(headers2Tick[3].name == "attitude", "HEADER DATA MISMATCH");
    KT_ASSERT(headers2Tick[3].dataType == KS_APOLLO_FLOAT, "HEADER NAMES DON'T MATCH");

    // Read data
    std::vector<std::vector<uint32_t>> data;
    std::vector<uint32_t> row;
    while (true) {
        row.clear();
        if (apolloImporter1Tick.ReadRow(row) != KS_SUCCESS)
            break;
        data.push_back(row);
    }

    uint32_t i = 0;
    for (auto& dataRow: data) {
        KT_ASSERT(row.size() == 4);
        for (auto value: dataRow) {
            KT_ASSERT(value == i++);
        }
    }

    data.clear();
    while (true) {
        row.clear();
        if (apolloImporter2Tick.ReadRow(row) != KS_SUCCESS)
            break;
        data.push_back(row);
    }

    i = 0;
    for (auto& dataRow: data) {
        KT_ASSERT(row.size() == 4);
        for (auto value: dataRow) {
            KT_ASSERT(value == i++);
        }
    }

    apolloImporter1Tick.Close();
    apolloImporter2Tick.Close();

    return true;
}
