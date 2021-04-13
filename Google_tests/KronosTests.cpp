#include "gtest/gtest.h"
#include "bus.h"
//#include "kronos.h"

using namespace kronos;

class KronosFixture : public ::testing::Test {

protected:
    //Kronos<int>* framework;
    //Kronos<int> framework;

    virtual void SetUp() {
      //  framework = new Kronos<int>();
    }

    virtual void TearDown() {
        //delete framework;
    }
};

TEST_F(KronosFixture, OpcodeValidity){
    auto* bus = new Bus<int>(1, "S");
}