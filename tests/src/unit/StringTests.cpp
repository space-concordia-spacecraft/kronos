#define CATCH_CONFIG_MAIN

#include "ks_string.h"
#undef FAIL
#include "../../include/catch_amalgamated.hpp"
#include <iostream>


TEST_CASE("Size Method Test ", "[StringSizeTest]") {
    kronos::String str = "Kronos";
    CHECK(str.Size() == 6);
}

// str.concatenate
TEST_CASE("Concatenate Method Test ", "[StringConcatenateTest]") {
    kronos::String str = "Kronos";
    kronos::String str1 = str.Concatenate(str,"Cronus");
    CHECK(str1.Size() == 12);
    CHECK(str1 == "KronosCronus");
}

TEST_CASE("Comparison Test ", "[StringComparisonTest]") {
    kronos::String str = "Kronos";
    kronos::String str1 = "Cronus";
    CHECK(str != str1);
}

TEST_CASE("Assignment Test ", "[StringAssignmentTest]") {
    kronos::String str = "Kronos";
    kronos::String str1 = str.Concatenate(str, "Cronus");
    CHECK(str1.Size() == 12);
}

//str +
TEST_CASE("Addition Test ", "[StringAdditionTest]") {
    kronos::String str = "Kronos";
    kronos::String str1 = str + "Cronus";
    CHECK(str1 == "KronosCronus");
}

//str +=
TEST_CASE("Addition Assignment Test ", "[StringAdditionAssignemntTest]") {
    kronos::String str = "Kronos";
    kronos::String str1 = "Cronus";
    str += str1;

    CHECK(str == "KronosCronus");
}

//str []
TEST_CASE("Index Test ", "[StringIndexTest]") {
    kronos::String str = "Kronos";
    CHECK(str[0] == 'K');
    CHECK(str[5] == 's');
}