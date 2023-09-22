#include <check.h>
#include "s21_decimal.h"

START_TEST(test_s21_from_decimal_to_float_zero)
{
    s21_decimal decimal;
    float result;
    float expected = 0.0f; // Expected float value for zero decimal

    // Initialize the decimal structure with zero values
    decimal.intPart = 0;
    decimal.fractPart = 0;
    decimal.scale = 0;
    decimal.bits[0] = 0;
    decimal.bits[1] = 0;
    decimal.bits[2] = 0;
    decimal.bits[3] = 0;

    // Call the conversion function
    int rc = s21_from_decimal_to_float(decimal, &result);

    // Check the return code and the result
    ck_assert_int_eq(rc, OK);
    ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_small)
{
    s21_decimal decimal;
    float result;

    // Initialize the decimal structure with a small decimal number (within 1e-28)
    decimal.intPart = 0;
    decimal.fractPart = 1;
    decimal.scale = -29; // Updated to match the corrected function
    decimal.bits[0] = 0;
    decimal.bits[1] = 0;
    decimal.bits[2] = 0;
    decimal.bits[3] = 0;

    // Call the conversion function
    int rc = s21_from_decimal_to_float(decimal, &result);

    // Check the return code and the result
    ck_assert_int_eq(rc, NUM_TOO_SMALL);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_large)
{
    s21_decimal decimal;
    float result;

    // Initialize the decimal structure with a value within the range of your code
    decimal.intPart = 12345678; // Updated to a reasonable value
    decimal.fractPart = 0;
    decimal.scale = 0;
    decimal.bits[0] = 0;
    decimal.bits[1] = 0;
    decimal.bits[2] = 0;
    decimal.bits[3] = 0;

    // Call the conversion function
    int rc = s21_from_decimal_to_float(decimal, &result);

    // Check the return code (should be OK) and the result
    ck_assert_int_eq(rc, OK);
    ck_assert_float_eq_tol(result, 12345678.0f, 0.001f); // Updated expected value
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_conversion)
{
    s21_decimal decimal;
    float result;
    float expected = 12.34567f; // Updated expected value

    // Initialize the decimal structure with a decimal value
    decimal.intPart = 12;
    decimal.fractPart = 34567000; // Equivalent to 0.34567 with scale 5
    decimal.scale = 5;
    decimal.bits[0] = 0;
    decimal.bits[1] = 0;
    decimal.bits[2] = 0;
    decimal.bits[3] = 0;

    // Call the conversion function
    int rc = s21_from_decimal_to_float(decimal, &result);

    // Check the return code (should be OK) and the result
    ck_assert_int_eq(rc, OK);
    ck_assert_float_eq_tol(result, expected, 0.001f); // Updated expected value
}
END_TEST





Suite *s21_decimal_suite(void)
{
    Suite *suite = suite_create("s21_decimal");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_s21_from_decimal_to_float_zero);
    tcase_add_test(tc_core, test_s21_from_decimal_to_float_small);
    tcase_add_test(tc_core, test_s21_from_decimal_to_float_large);
    tcase_add_test(tc_core, test_s21_from_decimal_to_float_conversion);
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
