#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  clear_bits(result);
  int out = OK;
  int scale = get_scale(&value);
  int sign = get_sign(value);
  if (result == NULL) {
    out = 1;
  } else {
    for (int i = scale; i > 0; i--) {
      div_by_ten(&value);
    }
    *result = value;
    set_scale(result, 0);
    set_sign(result, sign);
  }
  return out;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  clear_bits(result);
  int out = OK;
  int sign = get_sign(value);
  s21_decimal tmp = {{1, 0, 0, 0}};
  int scale = get_scale(&value);
  if (result == NULL) {
    out = 1;
  } else {
    if (scale == 0) {
      *result = value;
    } else {
      s21_truncate(value, result);
      if (sign) {
        bit_add(*result, tmp, result);
        set_sign(result, sign);
      }
    }
  }
  return out;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  clear_bits(result);
  int out = OK;
  int sign = get_sign(value);
  if (result == NULL) {
    out = 1;
  } else {
    if (!sign) {
      sign = 1;
    } else {
      sign = 0;
    }
    *result = value;
    set_sign(result, sign);
  }
  return out;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  clear_bits(result);
  int out = OK;
  int sign = get_sign(value);
  set_sign(&value, 0);
  s21_decimal val_without_scale = {{0, 0, 0, 0}};
  s21_decimal half = {{5, 0, 0, 0}};
  set_scale(&half, 1);
  s21_decimal tmp = {{0, 0, 0, 0}};
  s21_decimal one = {{1, 0, 0, 0}};
  if (result == NULL) {
    out = 1;
  } else {
    s21_truncate(value, &val_without_scale);
    s21_sub(value, val_without_scale, &tmp);

    if (s21_is_greater_or_equal(tmp, half)) {
      s21_add(val_without_scale, one, result);
      set_scale(result, 0);
      set_sign(result, sign);
    } else {
      *result = val_without_scale;
      set_scale(result, 0);
      set_sign(result, sign);
    }
  }
  return out;
}