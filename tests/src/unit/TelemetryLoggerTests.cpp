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
    Vector<TelemetryChannel> channels1Tick;
    TelemetryChannel channel;
    channel.name = "temperature";
    channel.retrieveTelemetry = &GetValue1Tick;
    channels1Tick.Add(channel);
    /*
    channels1Tick.Add({"temperature", &GetValue1Tick });
    channels1Tick.Add({"batteryLevel", &GetValue1Tick });
    channels1Tick.Add({"radiation", &GetValue1Tick });
    channels1Tick.Add({"attitude", &GetValue1Tick });
     */


    Vector<TelemetryChannel> channels2Tick;
    TelemetryChannel channel2;
    channel2.name = "temperature";
    channel2.retrieveTelemetry = &GetValue2Tick;
    channels2Tick.Add(channel2);
    /*
    channels2Tick.Add({"temperature", &GetValue2Tick });
    channels2Tick.Add({"batteryLevel", &GetValue2Tick });
    channels2Tick.Add({"radiation", &GetValue2Tick });
    channels2Tick.Add({"attitude", &GetValue2Tick });
     */

    ComponentTelemetryLogger logger("telemetryLogger");
    logger.AddTelemetryGroup("telemetryLoggerTest1Tick", 1, channels1Tick);
    logger.AddTelemetryGroup("telemetryLoggerTest2Tick", 2, channels2Tick);

    EventMessage msg;
    msg.eventCode = KS_EVENT_CODE_RATE_GROUP_TICK;

    for (int i = 0; i < 420; i++) {
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
    KT_ASSERT(headers1Tick.Size() == 4, "HEADER SIZE DOESN'T MATCH");
    KT_ASSERT(headers1Tick[0].name == "temperature", "HEADER DATA MISMATCH");
    KT_ASSERT(headers1Tick[0].dataType == KS_APOLLO_FLOAT, "HEADER DATA MISMATCH");
    KT_ASSERT(headers1Tick[1].name == "battery_level", "HEADER DATA MISMATCH");
    KT_ASSERT(headers1Tick[1].dataType == KS_APOLLO_FLOAT, "HEADER NAMES DON'T MATCH");
    KT_ASSERT(headers1Tick[2].name == "radiation", "HEADER DATA MISMATCH");
    KT_ASSERT(headers1Tick[2].dataType == KS_APOLLO_FLOAT, "HEADER NAMES DON'T MATCH");
    KT_ASSERT(headers1Tick[3].name == "attitude", "HEADER DATA MISMATCH");
    KT_ASSERT(headers1Tick[3].dataType == KS_APOLLO_FLOAT, "HEADER NAMES DON'T MATCH");

    // Create importer
    ApolloImporter apolloImporter2Tick(file1Tick);
    auto headers2Tick = apolloImporter2Tick.GetHeaders();
    KT_ASSERT(headers2Tick.Size() == 4, "HEADER SIZE DOESN'T MATCH");
    KT_ASSERT(headers2Tick[0].name == "temperature", "HEADER DATA MISMATCH");
    KT_ASSERT(headers2Tick[0].dataType == KS_APOLLO_FLOAT, "HEADER DATA MISMATCH");
    KT_ASSERT(headers2Tick[1].name == "battery_level", "HEADER DATA MISMATCH");
    KT_ASSERT(headers2Tick[1].dataType == KS_APOLLO_FLOAT, "HEADER NAMES DON'T MATCH");
    KT_ASSERT(headers2Tick[2].name == "radiation", "HEADER DATA MISMATCH");
    KT_ASSERT(headers2Tick[2].dataType == KS_APOLLO_FLOAT, "HEADER NAMES DON'T MATCH");
    KT_ASSERT(headers2Tick[3].name == "attitude", "HEADER DATA MISMATCH");
    KT_ASSERT(headers2Tick[3].dataType == KS_APOLLO_FLOAT, "HEADER NAMES DON'T MATCH");

    // Read data
    Vector<uint32_t> data1Tick;
    apolloImporter1Tick.ReadRow(data1Tick);

    Vector<uint32_t> data2Tick;
    apolloImporter1Tick.ReadRow(data2Tick);

    for (int i = 0; i < 420; i++) {
        KT_ASSERT(data1Tick[i] == i, "DATA DOESN'T MATCH");
    }

    for (int i = 0; i < 210; i++) {
        KT_ASSERT(data2Tick[i] == i, "DATA DOESN'T MATCH");
    }

    apolloImporter1Tick.Close();
    apolloImporter2Tick.Close();

    return true;
}
