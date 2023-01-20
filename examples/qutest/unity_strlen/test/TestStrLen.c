#include "unity.h"      // Unit testing harness (Unity)
#include "my_strlen.h"  // Code/Component Under Test (CUT)

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_my_strlen_0_for_empty_string(void) {
    uint16_t len = my_strlen(""); // call the CUT
    TEST_ASSERT_EQUAL(0U, len);   // verify test "assertion"
}

void test_my_strlen_n_for_various_strings(void) {
    TEST_ASSERT_EQUAL(3U, my_strlen("Foo"));
    TEST_ASSERT_EQUAL(4U, my_strlen(" Foo"));
}

void test_my_strlen_n_for_unprintable_chars(void) {
    TEST_ASSERT_EQUAL(4U, my_strlen("\tFoo"));
    TEST_ASSERT_EQUAL(3U, my_strlen("\a\b\n"));
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_my_strlen_0_for_empty_string);
    RUN_TEST(test_my_strlen_n_for_various_strings);
    RUN_TEST(test_my_strlen_n_for_unprintable_chars);
    return UNITY_END();
}
