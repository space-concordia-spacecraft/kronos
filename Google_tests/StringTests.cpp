#include "gtest/gtest.h"
#include "ks_string.h"
#include <iostream>

class StringFixture : public ::testing::Test {

protected:
    kronos::String str = "Kronos";
    virtual void SetUp() {
       
    }

    virtual void TearDown() {

    }

};


//Matteo Testing string Code
TEST_F(StringFixture, StringSizeTest) {
    EXPECT_EQ(str.size(), 6) << "Wrong size!";
    
}

// str.concatenate
TEST_F(StringFixture, StringConcatenateTest) {
    kronos::String str1 = str.concatenate(str,"Cronus");
    EXPECT_EQ(str1.size(), 12) << "Wrong size!";
    EXPECT_EQ(str1, "KronosCronus") << "Incorrect concatenation!";
}

// str == / !=
TEST_F(StringFixture, StringComparisonTest) {
    kronos::String str1 = "Cronus";
    EXPECT_NE(str, str1) << "The strings are not the same";
}

//str =
TEST_F(StringFixture, StringAssignmentTest) {
    kronos::String str1 = str.concatenate(str, "Cronus");
    EXPECT_EQ(str1.size(), 12) << "Wrong size!";
}

//str +
TEST_F(StringFixture, StringAdditionTest) {
    kronos::String str1 = str + "Cronus" ;
    EXPECT_EQ(str1, "KronosCronus") << "Incorrect string addition!";
}

//str +=
TEST_F(StringFixture, StringAdditionAssignemntTest) {
    kronos::String str1 = "Cronus";
    str += str1;
    
    EXPECT_EQ(str, "KronosCronus") << "Incorrect string addition and assignment!";
}

//str []
TEST_F(StringFixture, StringIndexTest) {
    EXPECT_EQ(str[0], 'K') << "Incorrect string index!";
    EXPECT_EQ(str[5], 's') << "Incorrect string index!";
}
