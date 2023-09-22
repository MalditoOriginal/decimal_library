#include <check.h>
#include "s21_decimal.h"

START_TEST(test_is_equal) {
  s21_decimal num1, num2;
  int rc;

  // Тест двух равных чисел
  num1.intPart = 123;
  num1.fractPart = 456;
  num2.intPart = 123;
  num2.fractPart = 456;
  rc = s21_is_equal(num1, num2);
  ck_assert_int_eq(rc, TRUE);

  // Тест различных целых частей
  num1.intPart = 789;
  num1.fractPart = 123;
  num2.intPart = 456;
  num2.fractPart = 123;
  rc = s21_is_equal(num1, num2);
  ck_assert_int_eq(rc, FALSE);

  // Тест различных дробных частей
  num1.intPart = 123;
  num1.fractPart = 789;
  num2.intPart = 123;
  num2.fractPart = 456;
  rc = s21_is_equal(num1, num2);
  ck_assert_int_eq(rc, FALSE);

  // Тест различных целых и дробных частей
  num1.intPart = 789;
  num1.fractPart = 456;
  num2.intPart = 123;
  num2.fractPart = 123;
  rc = s21_is_equal(num1, num2);
  ck_assert_int_eq(rc, FALSE);
} END_TEST

int main(void) {
  Suite *s;
  TCase *tc;
  SRunner *sr;

  s = suite_create("s21_decimal");
  tc = tcase_create("comparison");

  tcase_add_test(tc, test_is_equal);

  suite_add_tcase(s, tc);
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (failed == 0) ? 0 : 1;
}
