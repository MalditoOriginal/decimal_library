#include <check.h>
#include "s21_decimal.h"

START_TEST(s21_test_positive) {
    int src = 12345;
    s21_decimal result;
    int rc = s21_from_int_to_decimal(src, &result);
    ck_assert_int_eq(rc, OK);
    ck_assert_int_eq(result.intPart, 12345ULL);
    ck_assert_int_eq(result.fractPart, 0ULL);
    ck_assert_int_eq(result.scale, 0);
    // Add more assertions if needed
}
END_TEST

START_TEST(s21_test_negative) {
    int src = -12345;
    s21_decimal result;
    int rc = s21_from_int_to_decimal(src, &result);
    ck_assert_int_eq(rc, OK);
    ck_assert_int_eq(result.intPart, 12345ULL);
    ck_assert_int_eq(result.fractPart, 0ULL);
    ck_assert_int_eq(result.scale, 0);
    // Add more assertions if needed
}
END_TEST

Suite *s21_decimal_suite(void) {
    Suite *suite = suite_create("s21_decimal");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, s21_test_positive);
    tcase_add_test(tc_core, s21_test_negative);
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