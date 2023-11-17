#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (dst == NULL) {
    return CONV_ERR;
  }

  if (isinf(src) || isnan(src)) {
    return CONV_ERR;
  }

  clear_bits(dst);

  if (src < 0) {
    set_sign(dst, 1);
    src *= -1;  // Make src positive
  }

  dst->bits[0] = src;

  return OK;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (dst == NULL) {
    return CONV_ERR;
  }

  int result = OK;
  int sign = get_sign(src);

  if (src.bits[2] != 0 || src.bits[1] != 0 || sign) {
    result = CONV_ERR;
  } else {
    *dst = src.bits[0];
    if (sign == 1) {
      *dst *= -1;
    }
  }

  return result;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int result = OK;
  int scale = get_scale(&src);

  if (dst == NULL || scale > 28) {
    result = CONV_ERR;
  } else {
    *dst = 0.0;
    double temp = *dst;
    for (int i = 0; i < 96; i++) {
      if (get_bit(&src, i)) {
        temp += pow(2, i - 32);  // Fixed the exponent calculation
      }
    }
    while (scale > 0) {
      temp /= 10;
      scale--;
    }
    *dst = (float)temp;  // Cast temp back to float
    if (get_sign(src)) {
      *dst *= -1;
    }
  }
  return result;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  clear_bits(dst);
  int result = 0;
  if (isinf(src) == 1 || isnan(src) == 1 || dst == NULL) {
    result = 1;
  } else {
    int sign = 0;
    if (src < 0) {
      sign = 1;
    }
    int float_exp = get_float_exp(&src);
    float tmp = fabs(src);
    int powten = 0;
    while (powten < 28 && (int)tmp / (int)pow(2, 21) == 0) {
      tmp *= 10;
      powten++;
    }
    tmp = round(tmp);
    if (powten <= 28 && (float_exp > -94 && float_exp < 96)) {
      while (fmod(tmp, 10) == 0 && powten > 0) {
        powten--;
        tmp /= 10;
      }
      unsigned int uns_int = 0;
      float_exp = get_float_exp(&tmp);
      dst->bits[float_exp / 32] |= 1 << float_exp % 32;
      for (int i = float_exp - 1, j = 22; j >= 0; i--, j--) {
        if ((uns_int & (1 << j)) != 0) {
          dst->bits[i / 32] |= 1 << i % 32;
        }
      }
      set_sign(dst, sign);
      set_scale(dst, powten);
    }
  }
  return result;
}