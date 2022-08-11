#include <stdint.h>
#include "LedBar.h"
#include "unity.h"
#include "MockLed.h"

void setUp(void) {
    /* This is run before EACH TEST */
}

void tearDown(void) {
    /* This is run after EACH TEST */
}

void test_LedBar_0_percent_all_off(void) {
    Led_off_Expect(0U);
    Led_off_Expect(1U);
    Led_off_Expect(2U);
    Led_off_Expect(3U);
    Led_off_Expect(4U);

    LedBar_setPercent(0U);
}

void test_LedBar_100_percent_all_on(void) {
    Led_on_ExpectAndReturn(0U, 10U);
    Led_on_ExpectAndReturn(1U, 20U);
    Led_on_ExpectAndReturn(2U, 10U);
    Led_on_ExpectAndReturn(3U, 20U);
    Led_on_ExpectAndReturn(4U, 10U);

    TEST_ASSERT_EQUAL(70U, LedBar_setPercent(100U));
}

void test_LedBar_200_percent_ASSERT(void) {
    /* NOTE: Here we EXPECT breaking the internal assertion inside CUT.
     * But Unity does not provide a mechanism to verify that and
     * to PASS the test, so this test will FAIL...
     */
    TEST_ASSERT(LedBar_setPercent(200U));
}

void test_LedBar_19_percent_all_off(void) {
    Led_off_Expect(0U);
    Led_off_Expect(1U);
    Led_off_Expect(2U);
    Led_off_Expect(3U);
    Led_off_Expect(4U);

    TEST_ASSERT_EQUAL(0U, LedBar_setPercent(19U));
}

void test_LedBar_20_percent_one_on(void) {
    Led_on_ExpectAndReturn(0U, 10U);
    Led_off_Expect(1U);
    Led_off_Expect(2U);
    Led_off_Expect(3U);
    Led_off_Expect(4U);

    TEST_ASSERT_EQUAL(10U, LedBar_setPercent(20U));
}

void test_LedBar_50_percent_two_on(void) {
    Led_on_ExpectAndReturn(0U, 23U);
    Led_on_ExpectAndReturn(1U, 17U);
    Led_off_Expect(2U);
    Led_off_Expect(3U);
    Led_off_Expect(4U);

    TEST_ASSERT_EQUAL(40U, LedBar_setPercent(50U));
}

void test_LedBar_99_percent_four_on(void) {
    Led_on_ExpectAndReturn(0U, 10U);
    Led_on_ExpectAndReturn(1U, 20U);
    Led_on_ExpectAndReturn(2U, 10U);
    Led_on_ExpectAndReturn(3U, 20U);
    Led_off_Expect(4U);

    TEST_ASSERT_EQUAL(60U, LedBar_setPercent(99U));
}

/* assertion handler... */
void Q_onAssert(char const * const module, int const loc) {
    (void)module;
    UNITY_TEST_FAIL(loc, "ASSERTION");
}

/* NOTE:
* The Mock example requires a specific "test runner", which
* is provided in the file TestLedBar_Runner.c.
*/

