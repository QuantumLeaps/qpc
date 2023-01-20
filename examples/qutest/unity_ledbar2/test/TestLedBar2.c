#include <stdint.h>
#include "LedBar2.h"
#include "unity.h"
#include "MockLed.h"
#include "MockSpi.h"

void setUp(void) {
    // This is run before EACH TEST
}

void tearDown(void) {
    // This is run after EACH TEST
}

void test_LedBar_0_percent_all_off(void) {
    Led_off_Expect(0U);
    Led_off_Expect(1U);
    Led_off_Expect(2U);
    Led_off_Expect(3U);
    Led_off_Expect(4U);

    LedBar2_setPercent(0U);
}

void test_LedBar_100_percent_all_on(void)
{
    SPI_beginTransaction_Expect();
    Led_on_ExpectAndReturn(0U, 10U);
    SPI_transfer_Expect(10U);
    Led_on_ExpectAndReturn(1U, 20U);
    SPI_transfer_Expect(20U);
    Led_on_ExpectAndReturn(2U, 10U);
    SPI_transfer_Expect(10U);
    Led_on_ExpectAndReturn(3U, 20U);
    SPI_transfer_Expect(20U);
    Led_on_ExpectAndReturn(4U, 10U);
    SPI_transfer_Expect(10U);
    SPI_endTransaction_Expect();

    TEST_ASSERT_EQUAL(70U, LedBar2_setPercent(100U));
}

void test_LedBar_200_percent_ASSERT(void) {
    // NOTE: Here we EXPECT breaking the internal assertion inside CUT.
    // But Unity does not provide a mechanism to verify that and
    // to PASS the test, so this test will FAIL...
    //
    TEST_ASSERT(LedBar2_setPercent(200U));
}

void test_LedBar_19_percent_all_off(void) {
    Led_off_Expect(0U);
    Led_off_Expect(1U);
    Led_off_Expect(2U);
    Led_off_Expect(3U);
    Led_off_Expect(4U);

    TEST_ASSERT_EQUAL(0U, LedBar2_setPercent(19U));
}

void test_LedBar_20_percent_one_on(void) {
    SPI_beginTransaction_Expect();
    Led_on_ExpectAndReturn(0U, 10U);
    SPI_transfer_Expect(10U);
    SPI_endTransaction_Expect();
    Led_off_Expect(1U);
    Led_off_Expect(2U);
    Led_off_Expect(3U);
    Led_off_Expect(4U);

    TEST_ASSERT_EQUAL(10U, LedBar2_setPercent(20U));
}

void test_LedBar_50_percent_two_on_FLAWED(void) {
    Led_on_ExpectAndReturn(0U, 23U);
    Led_on_ExpectAndReturn(1U, 17U);
    Led_off_Expect(2U);
    Led_off_Expect(3U);
    Led_off_Expect(4U);

    SPI_beginTransaction_Expect();
    SPI_transfer_Expect(23U);
    SPI_transfer_Expect(17U);
    SPI_endTransaction_Expect();

    TEST_ASSERT_EQUAL(40U, LedBar2_setPercent(50U));
}

void test_LedBar_99_percent_four_on_FLAWED(void) {
    SPI_beginTransaction_Expect();
    Led_on_ExpectAndReturn(0U, 10U);
    SPI_transfer_Expect(10U);
    Led_on_ExpectAndReturn(1U, 20U);
    SPI_transfer_Expect(20);
    Led_on_ExpectAndReturn(2U, 10);
    SPI_transfer_Expect(10U);
    Led_on_ExpectAndReturn(3U, 20);
    SPI_transfer_Expect(20U);
    Led_off_Expect(4U);
    SPI_endTransaction_Expect();

    TEST_ASSERT_EQUAL(60U, LedBar2_setPercent(99U));
}

// error handler...
Q_NORETURN Q_onError(char const * const module, int_t const id) {
    Q_UNUSED_PAR(module);
    Q_UNUSED_PAR(id);

    UNITY_TEST_FAIL(loc, "ERROR");
}

// NOTE:
// The Mock example requires a specific "test runner", which
// is provided in the file TestLedBar_Runner.c.
//

