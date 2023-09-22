#include "s21_decimal.h"
#include <math.h>

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  // Обнуляем структуру результата
  dst->intPart = 0;
  dst->fractPart = 0;
  dst->scale = 0;
  dst->bits[0] = 0;
  dst->bits[1] = 0;
  dst->bits[2] = 0;
  dst->bits[3] = 0; // Обнуляем биты

  // Проверяем знак исходного числа
  if (src < 0) {
    src = -src;
    dst->intPart = (uint64_t)src;
    dst->bits[3] |= (1U << 31); // Устанавливаем бит знака
  } else {
    dst->intPart = (uint64_t)src;
  }

  // Возвращаем 0, если все ОК
  return OK;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    // Обнуляем структуру результата
    dst->intPart = 0;
    dst->fractPart = 0;
    dst->scale = 0;
    dst->bits[0] = 0;
    dst->bits[1] = 0;
    dst->bits[2] = 0;
    dst->bits[3] = 0; // Обнуляем биты

    // Проверяем, является ли число слишком маленьким или слишком большим
    if (src < -LARGE_NUM || src > LARGE_NUM || isnan(src) || isinf(src)) {
        return NUM_TOO_LARGE; // Слишком маленькое или слишком большое число
    }

    // Проверяем знак числа
    int isNegative = 0;
    if (src < 0) {
        isNegative = 1;
        src = -src; // Берем абсолютное значение
    }

    // Извлекаем целую и дробную части числа
    dst->intPart = (uint64_t)src;
    float fract = src - dst->intPart;

    // Преобразуем дробную часть в десятичное представление с округлением
if (fract != 0.0) {
    while (fract < 1.0) {
        fract *= 10;
        dst->scale++;
    }
    // Round the fractional part to the nearest integer
    fract += 0.5; // Add 0.5 for rounding
    dst->fractPart = (uint64_t)(fract); // Преобразуем дробную часть в целое число
}

    // Устанавливаем бит знака, если число отрицательное
    if (isNegative) {
        dst->bits[3] |= (1U << 31);
    }

    return OK;
}


int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (src.scale > 0) {
    return NUM_TOO_SMALL; // There is a fractional part, which we discard
  }
  
  // Check for integer overflow
  if (src.intPart > LARGE_NUM || src.intPart < SMALL_NUM) {
    return NUM_TOO_LARGE;
  }

  *dst = (int)src.intPart;
  return OK;
}


int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (src.intPart == 0 && src.fractPart == 0) {
    *dst = 0.0f; // The number is zero
  } else if (src.intPart == 0 && src.scale <= -28) {
    return NUM_TOO_SMALL; // The number is too small to represent as a float
  } else if (src.intPart >= LARGE_NUM || isnan(src.intPart) || isinf(src.intPart)) {
    return NUM_TOO_LARGE; // The number is too large or infinity
  } else {
    // Conversion of the integer part
    *dst = (float)src.intPart;

    // Conversion of the fractional part considering the scale
    uint64_t fractPart = src.fractPart;
    int scale = src.scale;
    while (scale < 0) {
      fractPart /= 10;
      scale++;
    }

    // Add the fractional part to the floating-point number
    *dst += (float)fractPart / S21_SIGN_DIG;
  }

  return OK;
}


