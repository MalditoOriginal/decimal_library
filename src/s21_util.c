#include "s21_decimal.h"

/* Returns the sign of the 'value' */
bool get_sign(s21_decimal value) {
  // Check the sign bit (most significant bit of the most significant word).
  return (value.bits[3] & SIGN_MASK) != 0;
}

/* Clears all bits in the 'result' */
void clear_bits(s21_decimal *result) {
  if (result != NULL) {
    for (int i = 0; i < 3; i++) {
      result->bits[i] = 0;
    }
  }
}

/* Returns the scale of the 'value' */
int get_scale(const s21_decimal *value) {
  if (value != NULL) {
    return value->scale;
  }
  return 0;  // Default value when value is NULL
}

int get_float_exp(float *src) {
  int exp = ((*(int *)src & ~SIGN_MASK) >> 23) - 127;
  return exp;
}

/* Sets the sign of 'result' to the specified 'sign' */
void set_sign(s21_decimal *result, int sign) {
  if (result != NULL) {
    if (sign < 0) {
      result->bits[3] |= 0x80000000;  // Set the sign bit to 1 for negative
    } else {
      result->bits[3] &= 0x7FFFFFFF;  // Set the sign bit to 0 for positive
    }
  }
}

/* Sets the scale of 'result' to the specified 'scale' */
void set_scale(s21_decimal *result, int scale) {
  if (result != NULL) {
    result->scale = scale;
  }
}

/* Divides 'value' by 10 */
void div_by_ten(s21_decimal *value) {
  if (value != NULL) {
    int carry = 0;
    for (int i = 0; i < 3; i++) {
      int temp = (value->bits[i] & 0x0FFFFFF) + (carry << 4);
      value->bits[i] = temp / 10;
      carry = temp % 10;
    }
    value->scale--;
  }
}

/* Adds two 's21_decimal' values and stores the result in antoher 's21_decimal'
 */
void bit_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // Initialize carry to 0.
  int carry = 0;

  for (int i = 0; i < 3; i++) {
    // Add corresponding bits from both values and the previous carry.
    int sum = value_1.bits[i] + value_2.bits[i] + carry;

    // Update the result and calculate the carry for the next iteration.
    result->bits[i] = sum % 1000000;  // Keep only the lower 6 digits.
    carry = sum / 1000000;            // Carry is the upper part of the sum.
  }

  // If there's still a carry after the loop, set it in the highest bit.
  if (carry != 0) {
    result->bits[3] |= 0x8000000;  // Set the sign bit to 1 for the negarive.
  } else {
    result->bits[3] &= 0x7FFFFFF;  // Set the sign bit to 0 for the positive.
  }
}

/* Extracts the value of a specific bit from an integer. */
int get_bit(s21_decimal *value, int bit_index) {
  if (bit_index < 0 || bit_index >= 96) {
    return 0;
  }

  int word_index = bit_index / 32;
  int bit_offset = bit_index % 32;

  return (value->bits[word_index] >> bit_offset) & 1;
}

// Assuming that the sign bit is in the most significant bit (bit 31)
int get_sign_bit(s21_decimal value) { return (value.bits[3] >> 31) & 1; }

void mul_by_ten(s21_decimal *value) {
  int carry = 0;
  for (int i = 0; i < 3; i++) {
    int new_word = (value->bits[i] * 10) + carry;
    value->bits[i] = new_word & 0xFFFFFFFF;  // Ensure 32-bit word
    carry = new_word >> 31;                  // Right-shift by 31 bits
  }

  // Handle any remaining carry
  value->bits[3] = (value->bits[3] * 10) + carry;
}

// Define the scale_equalization function
void scale_equalization(s21_decimal *value_1, s21_decimal *value_2) {
  if (get_scale(value_1) < get_scale(value_2)) {
    int difference = get_scale(value_2) - get_scale(value_1);
    if (get_bit(value_2, 93) == 0 && get_bit(value_2, 94) == 0 &&
        get_bit(value_2, 95) == 0) {
      for (int i = 0; i < difference; i++) {
        mul_by_ten(value_1);
      }
      set_scale(value_1, get_scale(value_2));
    } else {
      for (int i = 0; i < difference; i++) {
        div_by_ten(value_2);
      }
      set_scale(value_2, get_scale(value_1));
    }
  } else {
    int difference = get_scale(value_1) - get_scale(value_2);
    if (get_bit(value_1, 93) == 0 && get_bit(value_1, 94) == 0 &&
        get_bit(value_1, 95) == 0) {
      for (int i = 0; i < difference; i++) {
        mul_by_ten(value_2);
      }
      set_scale(value_2, get_scale(value_1));
    } else {
      for (int i = 0; i < difference; i++) {
        div_by_ten(value_1);
      }
      set_scale(value_1, get_scale(value_2));
    }
  }
}

int is_equal_without_sign(s21_decimal value_1, s21_decimal value_2) {
  int result;
  if (value_1.bits[2] != value_2.bits[2]) {
    result = FALSE;
  } else if (value_1.bits[1] != value_2.bits[1]) {
    result = FALSE;
  } else if (value_1.bits[0] != value_2.bits[0]) {
    result = FALSE;
  } else {
    result = TRUE;
  }
  return result;
}

int less_without_mod(const s21_decimal *value_1, const s21_decimal *value_2) {
  // Compare two s21_decimal values without using the modulus operation
  int i;

  for (i = 2; i >= 0; i--) {
    if (value_1->bits[i] < value_2->bits[i]) {
      return TRUE;
    } else if (value_1->bits[i] > value_2->bits[i]) {
      return FALSE;
    }
  }

  return FALSE;
}