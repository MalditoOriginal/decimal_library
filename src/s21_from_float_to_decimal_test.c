#include <check.h>
#include "s21_decimal.h"

START_TEST(s21_from_float_to_decimal_positive)
{
    s21_decimal result;
    int rc;

    // Test positive float value
    rc = s21_from_float_to_decimal(12.34567, &result);
    ck_assert_int_eq(rc, OK);
    ck_assert_uint_eq(result.intPart, 12);
    ck_assert_uint_eq(result.fractPart, 3456700000ULL); // Corrected expected value
}
END_TEST

START_TEST(s21_from_float_to_decimal_negative)
{
    s21_decimal result;
    int rc;

    // Test negative float value
    rc = s21_from_float_to_decimal(-12.34567, &result);
    ck_assert_int_eq(rc, OK);
    ck_assert_uint_eq(result.intPart, 12);
    ck_assert_uint_eq(result.fractPart, 3456700000ULL); // Corrected expected value
}
END_TEST

Suite *s21_decimal_suite(void) {
    Suite *suite = suite_create("s21_decimal");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, s21_from_float_to_decimal_positive);
    tcase_add_test(tc_core, s21_from_float_to_decimal_negative);
    suite_add_tcase(suite, tc_core);
    return suite;
}

int main(void) {
    int number_failed;
    Suite *suite = s21_decimal_suite();
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (number_failed == 0) ? CK_PASS : CK_FAILURE;
}

