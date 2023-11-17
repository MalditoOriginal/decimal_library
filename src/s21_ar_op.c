#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // Check for overflow
  if (value_1.bits[3] + value_2.bits[3] < value_1.bits[3]) {
    return 1;  // Overflow
  }

  // Add the bits
  result->bits[0] = value_1.bits[0] + value_2.bits[0];
  result->bits[1] = value_1.bits[1] + value_2.bits[1];
  result->bits[2] = value_1.bits[2] + value_2.bits[2];
  result->bits[3] = value_1.bits[3] + value_2.bits[3];

  // Check for overflow
  if (result->bits[0] < value_1.bits[0] || result->bits[1] < value_1.bits[1] ||
      result->bits[2] < value_1.bits[2] || result->bits[3] < value_1.bits[3]) {
    return 1;  // Overflow
  }

  return 0;  // No overflow
}

/*int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {

}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {

}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {

}
*/