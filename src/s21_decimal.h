#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LARGE_NUM 79228162514264337593543950335.0f
#define SMALL_NUM 1e-28
#define S21_SIGN_DIG 1e7f
#define SIGN_MASK 0x80000000

enum states {
  FALSE = 0,
  OK = 0,
  CALC_ERR = 1,
  TRUE = 1,
  CONV_ERR = 1,
  NUM_TOO_LARGE = 1,
  NUM_TOO_SMALL,
  DIV_BY_ZERO,
};

typedef struct {
  int bits[4];
  int scale;
} s21_decimal;

bool get_sign(s21_decimal value);
void clear_bits(s21_decimal *result);
int get_scale(const s21_decimal *value);
void set_sign(s21_decimal *result, int sign);
void set_scale(s21_decimal *result, int scale);
void div_by_ten(s21_decimal *value);
void bit_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int get_bit(s21_decimal *value, int bit_index);
int get_sign_bit(s21_decimal value);
void scale_equalization(s21_decimal *value_1, s21_decimal *value_2);
int is_equal_without_sign(s21_decimal value_1, s21_decimal value_2);
int less_without_mod(const s21_decimal *value_1, const s21_decimal *value_2);
int get_float_exp(float *src);

// Arithmetic Operators
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Comparison operators
int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

// Converters and parsers
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Another functions
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

#endif