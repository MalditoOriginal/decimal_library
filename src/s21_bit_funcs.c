#include "s21_decimal.h"

int bit_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // Clear the result initially
  clear_bits(result);

  // Initialize variables
  int ret_value = OK;
  int carry = 0;

  // Iterate through each bit
  for (int i = 0; i < 96; i++) {
    int bit_val_1 = get_bit(value_1, i);
    int bit_val_2 = get_bit(value_2, i);

    // Calculate the sum and carry
    int sum = bit_val_1 ^ bit_val_2 ^ carry;
    carry = (bit_val_1 & bit_val_2) | ((bit_val_1 ^ bit_val_2) & carry);

    // Set the result bit
    set_bit(result, i, sum);
  }

  // Check for overflow
  if (carry) {
    ret_value = INF;
    clear_bits(result);
  }

  return ret_value;
}

int bit_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // Clear the result initially
  clear_bits(result);

  // Initialize varibles
  int last_b = 1 + last_bit(value_1);
  int borrow = 0;

  // Iterate through each bit
  for (int i = 0; i < last_b; i++) {
    int bit_val_1 = get_bit(value_1, i);
    int bit_val_2 = get_bit(value_2, i);

    // Calculate the difference and borrow
    int diff = (bit_val_1 - borrow) - bit_val_2;
    borrow = (diff < 0) ? 1 : 0;

    // Set the result bit
    set_bit(result, i, (diff + 2) % 2);
  }

  return 0;
}

int bit_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // Clear the result initially
  clear_bits(result);

  // Initialize variables
  int ret_value = OK;
  s21_decimal tmp;

  // Iterate through each bit of the first value
  for (int i = 0; i <= last_bit(value_1); i++) {
    // If the current bit is set, add the second value offset to the result
    if (get_bit(value_1, i)) {
      // Initialize tmp with value_2 and left shift it by i
      clear_bits(&tmp);
      tmp = value_2;
      ret_value = offset_left(&tmp, i);

      // Add tmp to the result
      bit_add(*result, tmp, result);
    }
  }

  return ret_value;
}

void bit_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
             s21_decimal *tmp) {
  // Clear the result and tmp initially
  clear_bits(result);
  clear_bits(tmp);

  // Iterate through each bit of the dividend (value_1)
  for (int i = last_bit(value_1); i >= 0; i--) {
    // Left shift tmp and set the least significant bit based on the current
    // dividend bit
    offset_left(tmp, 1);
    set_bit(tmp, 0, get_bit(value_1, i));

    // Check if tmp is greater or equal to the divisor (value_2)
    if (s21_is_greater_or_equal(*tmp, value_2)) {
      // Subtract value_2 from tmp
      bit_sub(*tmp, value_2, tmp);

      // Left shift the result and set the least significant bit
      offset_left(result, 1);
      set_bit(result, 0, 1);
    } else {
      // If not, only left shift the result
      offset_left(result, 1);
    }
  }
}

void bits_copy(s21_decimal src, s21_decimal *dest) {
  dest->bits[0] = src.bits[0];
  dest->bits[1] = src.bits[1];
  dest->bits[2] = src.bits[2];
}