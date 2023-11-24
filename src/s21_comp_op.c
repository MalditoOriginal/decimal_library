#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  s21_decimal zero = {{0, 0, 0, 0}};

  // Get signs and scales of the values
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);

  // Ensure equal scales
  if (get_scale(&value_1) != get_scale(&value_2))
    scale_equal(&value_1, &value_2);

  // Check if both values are zero
  if (is_equal_without_sign(value_1, zero) &&
      is_equal_without_sign(value_2, zero)) {
    return FALSE;
  }

  // Compare signs and values without considering sign
  int result = (!sign_1 && sign_2)   ? FALSE
               : (sign_1 && !sign_2) ? TRUE
                                     : mod_comp(value_1, value_2);

  // Adjust result based on signs
  return (sign_1 && sign_2) ? !result : result;
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
  int ret_value;
  if (get_scale(&value_1) != get_scale(&value_2))
    scale_equal(&value_1, &value_2);

  s21_decimal zero = {{0, 0, 0, 0}};  // Initialize the 'scale' field to 0

  if (is_equal_without_sign(zero, value_1) &&
      is_equal_without_sign(zero, value_2)) {
    ret_value = TRUE;  // If both values are zero, they are equal
  } else if (get_sign(value_1) != get_sign(value_2)) {
    ret_value = FALSE;  // If signs are different, values are not equal
  } else {
    ret_value = TRUE;

    // Iterate through bits and check for equality
    for (int i = 2; i >= 0; i--) {
      if (value_1.bits[i] != value_2.bits[i]) {
        ret_value = FALSE;  // If a bit mismatch is found, values are not equal
        break; /* Exit the loop early since the values are not equals */
      }
    }
  }

  return ret_value;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}
