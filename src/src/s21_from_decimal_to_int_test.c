#include <check.h>
#include "s21_decimal.h"

START_TEST(test_s21_from_decimal_to_int_valid)
{
    s21_decimal decimal;
    int result;
    int expected = 123; // Expected integer value

    // Initialize the decimal structure with a valid integer value and no fractional part
    decimal.intPart = 123;
    decimal.fractPart = 0;
    decimal.scale = 0;
    decimal.bits[0] = 0;
    decimal.bits[1] = 0;
    decimal.bits[2] = 0;
    decimal.bits[3] = 0;

    // Call the conversion function
    int rc = s21_from_decimal_to_int(decimal, &result);

    // Check the return code and the result
    ck_assert_int_eq(rc, OK);
    ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_with_fraction)
{
    s21_decimal decimal;
    int result;

    // Initialize the decimal structure with a fractional part
    decimal.intPart = 123;
    decimal.fractPart = 456;
    decimal.scale = 3; // Indicate that there is a fractional part
    decimal.bits[0] = 0;
    decimal.bits[1] = 0;
    decimal.bits[2] = 0;
    decimal.bits[3] = 0;

    // Call the conversion function
    int rc = s21_from_decimal_to_int(decimal, &result);

    // Check the return code (should be NUM_TOO_SMALL) and the result (undefined)
    ck_assert_int_eq(rc, NUM_TOO_SMALL);
}
END_TEST

Suite *s21_decimal_suite(void)
{
    Suite *suite = suite_create("s21_decimal");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_s21_from_decimal_to_int_valid);
    tcase_add_test(tc_core, test_s21_from_decimal_to_int_with_fraction);
    suite_add_tcase(suite, tc_core);
    return suite;
}

int main(void)
{
    int number_failed;
    Suite *suite = s21_decimal_suite();
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (number_failed == 0) ? CK_PASS : CK_FAILURE;
}
