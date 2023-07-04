#include "s21_decimal.h"
#include <math.h>

// Converters and parsers

int s21_from_int_to_decimal(int src, s21_decimal* dst) {
    dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
    dst->bits[0] = src;

    return src;
}

int s21_from_float_to_decimal(float src, s21_decimal* dst) {
    if (fabs(src) > 0 && fabs(src) < SMALL_NUM)
        return CONV_ERR;
    if (fabs(src) > LARG_NUM || isinf(src))
        return CONV_ERR;

    // Convert to decimal
    float rounded = roundf(src * SIGN_DIG) / SIGN_DIG;
    int integer = (int)rounded;

    // Discard fractional part
    dst->bits[0] = integer % 10;
    dst->bits[1] = (integer / 10) % 10;
    dst->bits[2] = (integer / 100) % 10;
    dst->bits[3] = (integer / 1000) % 10;

    return OK;
}


int s21_from_decimal_to_float(s21_decimal src, float* dst) {
    // Convert decimal to float
    float result = (float)src.bits[0] +
                   (float)(src.bits[1] * 10) +
                   (float)(src.bits[2] * 100) +
                   (float)(src.bits[3] * 1000);

    *dst = result;

    return OK;
}

int s21_from_decimal_to_int(s21_decimal src, int* dst) {
    // Calculate the integer value from the decimal representation
    int value = src.bits[3] * 1000 + src.bits[2] * 100 + src.bits[1] * 10 + src.bits[0];
    
    *dst = value;  // Store the result in the dst parameter

    return 0;  // OK
}

