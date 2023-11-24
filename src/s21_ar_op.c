#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  clear_bits(result);
  int ret_value = OK;
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);

  // Ensure equal scales
  if (get_scale(&value_1) != get_scale(&value_2)) {
    scale_equal(&value_1, &value_2);
  }

  if (sign_1 == sign_2) {
    if (!sign_1) {
      // a + b
      ret_value = bit_add(value_1, value_2, result);
      set_sign(result, 0);
    } else {
      // (-a) + (-b)
      ret_value = bit_add(value_1, value_2, result);
      if (ret_value == INF) {
        ret_value = N_INF;
      }
      set_sign(result, 1);
    }
  } else if (sign_1 < sign_2) {
    // a + (-b)
    if (mod_comp(value_1, value_2)) {
      ret_value = bit_sub(value_2, value_1, result);
      set_sign(result, 1);
    } else {
      ret_value = bit_sub(value_1, value_2, result);
      set_sign(result, 0);
    }
  } else {
    // (-a) + b
    if (mod_comp(value_1, value_2)) {
      ret_value = bit_sub(value_2, value_1, result);
      set_sign(result, 0);
    } else {
      ret_value = bit_sub(value_1, value_2, result);
      set_sign(result, 1);
    }
  }

  set_scale(result, get_scale(&value_1));
  return ret_value;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  clear_bits(result);
  int ret_value = OK;
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);

  // Ensure equal scales
  if (get_scale(&value_1) != get_scale(&value_2)) {
    scale_equal(&value_1, &value_2);
  }

  if (sign_1 != sign_2) {
    // Different signs: a - b = a + (-b)
    set_sign(&value_1, 0);
    set_sign(&value_2, 0);
    ret_value = s21_add(value_1, value_2, result);
    if (ret_value == INF) {
      ret_value = N_INF;
    } else {
      set_sign(result, sign_1);
    }
  } else {
    // Same signs
    set_sign(&value_1, 0);
    set_sign(&value_2, 0);

    // Determine which value is greater
    s21_decimal *big_val, *small_val;
    int sign;
    if (s21_is_less(value_1, value_2)) {
      small_val = &value_1;
      big_val = &value_2;
      sign = !sign_2;
    } else {
      small_val = &value_2;
      big_val = &value_1;
      sign = sign_1;
    }

    // Calculate the subtraction
    bit_sub(*big_val, *small_val, result);
    set_scale(result, get_scale(&value_1));
    set_sign(result, sign);
  }

  return ret_value;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // Clear bits in the result
  clear_bits(result);

  // Get signs of values
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);

  // Set the sign of the result based on the signs of the operands
  set_sign(result, sign_1 ^ sign_2);

  // Multiply the absolute values of the operands
  int ret_value = bit_mul(value_1, value_2, result);

  // Set the scale of the result
  set_scale(result, get_scale(&value_1) + get_scale(&value_2));

  return ret_value;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  clear_bits(result);
  s21_decimal zero = {{0, 0, 0, 0}};
  s21_decimal tmp = {{0, 0, 0, 0}};
  s21_decimal remember = {{0, 0, 0, 0}};
  int sign = result_sign(value_1, value_2);
  int ret_value = OK;

  if (s21_is_equal(value_2, zero) == FALSE) {
    int scale = get_scale(&value_1) - get_scale(&value_2);
    set_scale(&value_1, 0);
    set_scale(&value_2, 0);
    set_sign(&value_1, 0);
    set_sign(&value_2, 0);
    bit_div(value_1, value_2, &tmp, &remember);
    bits_copy(tmp, result);
    s21_decimal max_value = {{4294967295u, 4294967295u, 4294967295u, 0}};
    set_scale(&max_value, 1);
    int iscale = 0;

    while (iscale < 28 && s21_is_equal(remember, zero) != TRUE) {
      if (s21_is_less(*result, max_value) != TRUE) break;
      mul_by_ten(&remember);
      bit_div(remember, value_2, &tmp, &remember);
      mul_by_ten(result);
      bit_add(*result, tmp, result);
      iscale++;
    }

    int new_scale = scale + iscale;

    while (new_scale > 28) {
      div_by_ten(result);
      new_scale--;
    }

    while (new_scale < 0) {
      mul_by_ten(result);
      new_scale++;
    }

    set_sign(result, sign);
    set_scale(result, new_scale);
  } else {
    ret_value = N_NAN;
    clear_bits(result);
  }

  return ret_value;
}