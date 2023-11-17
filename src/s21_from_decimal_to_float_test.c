#include "s21_decimal.h"

START_TEST(s21_from_decimal_to_float_1) {
  float src = 0.00000000000;
  float temp = 0;
  int return_value = 0;
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_from_decimal_to_float(value_1, &temp);
  if (fabs(src - temp) < pow(10, -7)) {
    return_value = 1;
  }
  ck_assert_int_eq(return_value, 1);
}
END_TEST
/*
START_TEST(s21_from_decimal_to_float_2) {
  float src = 0.01;
  float temp = 0;
  int return_value = 0;
  s21_decimal value_1 = {{1, 0, 0, 0}};
  value_1.bits[3] += 65536 * 2;
  s21_from_decimal_to_float(value_1, &temp);
  if (fabs((src - temp)) < pow(10, -7)) {
    return_value = 1;
  }
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(s21_from_decimal_to_float_3) {
  float src = 0.00000123;
  float temp = 0;
  int return_value = 0;
  s21_decimal value_1 = {{123, 0, 0, 0}};
  value_1.bits[3] += 262144 * 2;
  s21_from_decimal_to_float(value_1, &temp);
  if (fabs((src - temp)) < pow(10, -7)) {
    return_value = 1;
  }
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(s21_from_decimal_to_float_4) {
  float src = -0.01;
  float temp = 0;
  int return_value = 0;
  s21_decimal value_1 = {{1, 0, 0, 0x80020000}};
  s21_from_decimal_to_float(value_1, &temp);
  if (fabs((src - temp) / src) < pow(10, -7)) {
    return_value = 1;
  }
  ck_assert_int_eq(return_value, 1);
}
END_TEST
*/
int main(void) {
  Suite *s;
  TCase *tc;
  SRunner *sr;

  s = suite_create("s21_decimal");
  tc = tcase_create("convertation");

  tcase_add_test(tc, s21_from_decimal_to_float_1);
  // tcase_add_test(tc, s21_from_decimal_to_float_2);
  // tcase_add_test(tc, s21_from_decimal_to_float_3);
  // tcase_add_test(tc, s21_from_decimal_to_float_4);

  suite_add_tcase(s, tc);
  sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (failed == 0) ? 0 : 1;
}