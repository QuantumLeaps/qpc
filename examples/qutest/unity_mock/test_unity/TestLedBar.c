#include <stdint.h>
#include "LedBar.h"
#include "unity.h"
#include "MockLed.h"

void setUp(void)
{
    /* This is run before EACH TEST */
}

void tearDown(void)
{
    /* This is run after EACH TEST */
}

void test_LedBar_0_percent_all_off(void)
{
    Led_off_Expect(0);
    Led_off_Expect(1);
    Led_off_Expect(2);
    Led_off_Expect(3);
    Led_off_Expect(4);

    LedBar_setPercent(0);
}

void test_LedBar_100_percent_all_on(void)
{
    Led_on_ExpectAndReturn(0, 10);
    Led_on_ExpectAndReturn(1, 20);
    Led_on_ExpectAndReturn(2, 10);
    Led_on_ExpectAndReturn(3, 20);
    Led_on_ExpectAndReturn(4, 10);

    TEST_ASSERT_EQUAL(70, LedBar_setPercent(100));
}

void test_LedBar_19_percent_all_off(void)
{
    Led_off_Expect(0);
    Led_off_Expect(1);
    Led_off_Expect(2);
    Led_off_Expect(3);
    Led_off_Expect(4);

    TEST_ASSERT_EQUAL(0, LedBar_setPercent(19));
}

void test_LedBar_20_percent_one_on(void)
{
    Led_on_ExpectAndReturn(0, 10);
    Led_off_Expect(1);
    Led_off_Expect(2);
    Led_off_Expect(3);
    Led_off_Expect(4);

    TEST_ASSERT_EQUAL(10, LedBar_setPercent(20));
}

void test_LedBar_50_percent_two_on(void)
{
    Led_on_ExpectAndReturn(0, 23);
    Led_on_ExpectAndReturn(1, 17);
    Led_off_Expect(2);
    Led_off_Expect(3);
    Led_off_Expect(4);

    TEST_ASSERT_EQUAL(40, LedBar_setPercent(50));
}

void test_LedBar_99_percent_four_on(void)
{
    Led_on_ExpectAndReturn(0, 10);
    Led_on_ExpectAndReturn(1, 20);
    Led_on_ExpectAndReturn(2, 10);
    Led_on_ExpectAndReturn(3, 20);
    Led_off_Expect(4);

    TEST_ASSERT_EQUAL(60, LedBar_setPercent(99));
}


/* NOTE:
* The Mock example requires a specific "test runner", which
* is provided in the file TestLedBar_Runner.c.
*/

