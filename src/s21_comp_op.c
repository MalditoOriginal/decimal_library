#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int result;
  s21_decimal zero = {{0, 0, 0, 0}, 0};
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);

  if (get_scale(&value_1) != get_scale(&value_2))
    scale_equalization(&value_1, &value_2);

  if (is_equal_without_sign(value_1, zero) == TRUE &&
      is_equal_without_sign(value_2, zero) == TRUE) {
    result = FALSE;
  } else if (!sign_1 && sign_2) {
    result = FALSE;
  } else if (sign_1 && !sign_2) {
    result = TRUE;
  } else {
    result = less_without_mod(&value_1, &value_2);
    if (sign_1 && sign_2 && result == TRUE) {
      result = FALSE;
    } else if (sign_1 && sign_2 && result == FALSE) {
      result = TRUE;
    }
  }

  return result;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_less(value_2, value_1);  // Swap value_1 and value_2
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return (s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2));
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return (s21_is_greater(value_1, value_2) || s21_is_equal(value_1, value_2));
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int result;
  if (get_scale(&value_1) != get_scale(&value_2))
    scale_equalization(&value_1, &value_2);
  s21_decimal zero = {{0, 0, 0, 0}, 0};  // Initialize the 'scale' field to 0
  if (is_equal_without_sign(zero, value_1) == TRUE &&
      is_equal_without_sign(zero, value_2) == TRUE) {
    result = TRUE;
  } else if (get_sign(value_1) != get_sign(value_2)) {
    result = FALSE;
  } else if (value_1.bits[2] != value_2.bits[2]) {
    result = FALSE;
  } else if (value_1.bits[1] != value_2.bits[1]) {
    result = FALSE;
  } else if (value_1.bits[0] != value_2.bits[0]) {
    result = FALSE;
  } else {
    result = TRUE;
  }
  return result;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}
