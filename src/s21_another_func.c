#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (result == NULL) {
    return CALC_ERR;
  }

  clear_bits(result);
  int return_value = OK;
  int scale = get_scale(&value);
  int sign = get_sign(value);

  for (int i = scale; i > 0; i--) {
    div_by_ten(&value);
  }

  *result = value;
  set_scale(result, 0);
  set_sign(result, sign);

  return return_value;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (result == NULL) {
    return CALC_ERR;
  }

  clear_bits(result);
  int return_value = OK;
  int sign = get_sign(value);
  s21_decimal temp = {{0, 0, 0}, 0};  // Initialize scale to 0
  int scale = get_scale(&value);

  if (scale == 0) {
    *result = value;
  } else {
    s21_truncate(value, result);
    if (sign) {
      bit_add(*result, temp, result);
      set_sign(result, sign);
    }
  }

  return return_value;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  if (result == NULL) {
    return CALC_ERR;
  }
  clear_bits(result);
  int sign = get_sign(value);

  if (!sign) {
    sign = 1;
  } else {
    sign = 0;
  }

  *result = value;
  set_sign(result, sign);

  return OK;
}

/*
int s21_round(s21_decimal value, s21_decimal *result) {
   clear_bits(result);
   int return_value = OK;
   int sign = get_sign(value);
   set_sign(&value, 0);
   s21_decimal val_without_scale = {{0, 0, 0, 0}, 0};
   s21_decimal five = {{5, 0, 0, 0}, 0};
   set_scale(&five, 1);
   s21_decimal temp = {{0, 0, 0, 0}, 0};
   s21_decimal one = {{1, 0, 0, 0}, 0};

   if (result == NULL) {
     return CALC_ERR;
   } else {
     s21_truncate(value, &val_without_scale);
     s21_sub(value, val_without_scale, &temp);

     if (s21_is_greater_or_equal(temp, five)) {
       s21_add(val_without_scale, one, result);
       set_scale(result, 0);
       set_sign(result, sign);
     } else {
       *result = val_without_scale;
       set_scale(result, 0);
       set_sign(result, sign);
     }
   }

   return return_value;
}
*/