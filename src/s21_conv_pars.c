#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  clear_bits(dst);
  int res = OK;
  if (dst == NULL || isinf((float)src) || isnan((float)src)) {
    res = 1;
  } else {
    if (src < 0) {
      src *= -1;
      set_sign(dst, 1);
    }
    dst->bits[0] = src;
  }
  return res;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int out = OK;
  int sign = get_sign(src);
  int s = get_bit(src, 31);
  if (src.bits[2] != 0 || src.bits[1] != 0 || s == 1 || dst == NULL) {
    out = 1;
  } else {
    *dst = 0;
    for (int i = get_scale(&src); i > 0; i--) {
      div_by_ten(&src);
    }
    *dst = src.bits[0];
    if (sign == 1) {
      *dst *= -1;
    }
  }
  return out;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int out = 0;
  int scale = get_scale(&src);
  if (dst == NULL || scale > 28) {
    out = 1;
  } else {
    *dst = 0.0;
    double tmp = *dst;
    for (int i = 0; i < 96; i++) {
      if (get_bit(src, i)) {
        tmp += pow(2, i);
      }
    }
    while (scale > 0) {
      tmp /= 10;
      scale--;
    }
    *dst = tmp;
    if (get_sign(src)) {
      *dst *= -1;
    }
  }
  return out;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  clear_bits(dst);
  int result = 0;
  if (isinf((float)src) == 1 || isnan((float)src) == 1 || dst == NULL) {
    result = 1;
  } else {
    int sign = get_float_sign(src);
    int float_exp = get_float_exp(&src);
    float tmp = fabs(src);
    int powten = 0;
    while (powten < 28 && (int)tmp / (int)pow(2, 21) == 0) {
      tmp *= 10;
      powten++;
    }
    tmp = round(tmp);
    if (powten <= 28 && (float_exp > -94 && float_exp < 96)) {
      UnFloatUnion mantissa;
      while (fmod(tmp, 10) == 0 && powten > 0) {
        powten--;
        tmp /= 10;
      }
      mantissa.float_pt = tmp;
      float_exp = get_float_exp(&mantissa.float_pt);
      dst->bits[float_exp / 32] |= 1 << float_exp % 32;
      for (int i = float_exp - 1, j = 22; j >= 0; i--, j--) {
        if ((mantissa.unsign_int & (1 << j)) != 0) {
          dst->bits[i / 32] |= 1 << i % 32;
        }
      }
      set_sign(dst, sign);
      set_scale(dst, powten);
    }
  }
  return result;
}