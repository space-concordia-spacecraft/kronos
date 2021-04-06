#include "gtest/gtest.h"
#include "hashmap.h"
#include "keyhash.h"

//using namespace kronos;

class KronosFixture : public ::testing::Test {

protected:
    //Kronos<int>* framework;

    virtual void SetUp() {
        //framework = new Kronos<int>();
    }

    virtual void TearDown() {
        //delete framework;
    }
};

TEST_F(KronosFixture, OpcodeValidity){
    //auto* bus = new Bus<int>(1, "S");
    //framework->registerBus(1, *bus);
    //int i = framework->isValidOpcode(0).code;
    //EXPECT_EQ(framework->isValidOpcode(0).code,framework->RESERVED_OPCODE.code);
}