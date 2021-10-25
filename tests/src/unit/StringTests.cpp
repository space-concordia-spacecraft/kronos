#include "KronosTest.h"
#include "ks_string.h"

KT_TEST(StringSizeTest) {
    kronos::String str = "Kronos";

    KT_ASSERT(str.Size() == 6);

    return true;
}

KT_TEST(StringConcatenateTest) {
    kronos::String str = "Kronos";
    kronos::String str1 = str.Concatenate(str, "Cronus");

    KT_ASSERT(str1.Size() == 12);
    KT_ASSERT(str1 == "KronosCronus");

    return true;
}

KT_TEST(StringSubstringTest) {
    kronos::String str = "Kronos";
    kronos::String str1 = str.Substring(0, 2);

    KT_ASSERT(str1 == "Kr");
    KT_ASSERT(str1.Size() == 2);

    return true;
}

KT_TEST(StringComparisonTest) {
    kronos::String str = "Kronos";
    kronos::String str1 = "Cronus";

    KT_ASSERT(str != str1);

    return true;
}

KT_TEST(StringAssignmentTest) {
    kronos::String str = "Kronos";
    kronos::String str1 = str.Concatenate(str, "Cronus");

    KT_ASSERT(str1.Size() == 12);

    return true;
}

KT_TEST(StringAdditionTest) {
    kronos::String str = "Kronos";
    kronos::String str1 = str + "Cronus";

    KT_ASSERT(str1 == "KronosCronus");

    return true;
}

KT_TEST(StringAdditionAssignmentTest) {
    kronos::String str = "Kronos";
    kronos::String str1 = "Cronus";
    str += str1;

    KT_ASSERT(str == "KronosCronus");

    return true;
}

KT_TEST(StringIndexTest) {
    kronos::String str = "Kronos";

    KT_ASSERT(str[0] == 'K');
    KT_ASSERT(str[5] == 's');

    return true;
}